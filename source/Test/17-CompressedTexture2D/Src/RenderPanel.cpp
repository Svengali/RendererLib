#include "RenderPanel.hpp"
#include "Application.hpp"

#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Renderer.hpp>
#include <Core/RenderingResources.hpp>
#include <Core/SwapChain.hpp>
#include <Enum/SubpassContents.hpp>
#include <Image/Texture.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <FileUtils.hpp>

#include <gli/gli.hpp>

#include <fstream>
#include <cstring>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			RenderTimer = 42
		}	Ids;

		static int const TimerTimeMs = 40;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & renderer )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_vertexData{
			{
				{
					{ -0.7f, -0.7f, 0.0f, 1.0f },
					{ -0.0f, -0.0f },
				},
				{
					{ -0.7f, 0.7f, 0.0f, 1.0f },
					{ -0.0f, 1.0f },
				},
				{
					{ 0.7f, -0.7f, 0.0f, 1.0f },
					{ 1.0f, -0.0f },
				},
				{
					{ 0.7f, 0.7f, 0.0f, 1.0f },
					{ 1.0f, 1.0f },
				}
		} }
	{
		try
		{
			doCreateDevice( renderer );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swapchain created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateTexture();
			std::cout << "Texture created." << std::endl;
			doCreateDescriptorSet();
			std::cout << "Descriptor set created." << std::endl;
			doCreateRenderPass();
			std::cout << "Render pass created." << std::endl;
			doCreateVertexBuffer();
			std::cout << "Vertex buffer created." << std::endl;
			doCreatePipeline();
			std::cout << "Pipeline created." << std::endl;
			doPrepareFrames();
			std::cout << "Frames prepared." << std::endl;
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}

		m_timer->Start( TimerTimeMs );

		Connect( int( Ids::RenderTimer ), wxEVT_TIMER, wxTimerEventHandler( RenderPanel::onTimer ), nullptr, this );
		Connect( wxID_ANY, wxEVT_SIZE, wxSizeEventHandler( RenderPanel::onSize ), nullptr, this );
	}

	RenderPanel::~RenderPanel()
	{
		doCleanup();
	}

	void RenderPanel::doCleanup()
	{
		delete m_timer;

		if ( m_device )
		{
			m_device->waitIdle();
			m_queryPool.reset();
			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_sampler.reset();
			m_view.reset();
			m_texture.reset();
			m_stagingBuffer.reset();
			m_pipeline.reset();
			m_vertexLayout.reset();
			m_pipelineLayout.reset();
			m_vertexBuffer.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_renderPass.reset();
			m_swapChain.reset();
			m_device->disable();
			m_device.reset();
		}
	}

	void RenderPanel::doCreateDevice( renderer::Renderer const & renderer )
	{
		m_device = renderer.createDevice( common::makeConnection( this, renderer ) );
		m_device->enable();
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( { uint32_t( size.x ), uint32_t( size.y ) } );
		m_swapChain->setClearColour( renderer::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doResetSwapChain();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string assetsFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Assets";
		gli::texture2d tex2D;
		renderer::Format format;

		if ( m_device->getFeatures().textureCompressionASTC_LDR )
		{
			format = renderer::Format::eASTC_8x8_UNORM_BLOCK;
			tex2D = gli::texture2d( gli::load( assetsFolder / "stonefloor01_color_astc_8x8_unorm.ktx" ) );
		}
		else if ( m_device->getFeatures().textureCompressionBC )
		{
			format = renderer::Format::eBC3_UNORM_BLOCK;
			tex2D = gli::texture2d( gli::load( assetsFolder / "stonefloor01_color_bc3_unorm.ktx" ) );
		}
		else if ( m_device->getFeatures().textureCompressionETC2 )
		{
			format = renderer::Format::eETC2_R8G8B8_UNORM_BLOCK;
			tex2D = gli::texture2d( gli::load( assetsFolder / "stonefloor01_color_etc2_unorm.ktx" ) );
		}
		else
		{
			throw std::runtime_error{ "No compressed texture format supported." };
		}

		// Create a host-visible staging buffer that contains the raw image data
		renderer::BufferBasePtr stagingBuffer = m_device->createBuffer( uint32_t( tex2D.size() )
			, renderer::BufferTarget::eTransferSrc
			, renderer::MemoryPropertyFlag::eHostVisible | renderer::MemoryPropertyFlag::eHostCoherent );

		// Copy texture data into staging buffer
		uint8_t * data = stagingBuffer->lock( 0u
			, stagingBuffer->getSize()
			, renderer::MemoryMapFlag::eWrite );
		memcpy( data, tex2D.data(), tex2D.size() );
		stagingBuffer->unlock();

		// Create the texture image
		m_texture = m_device->createTexture(
			{
				0u,
				renderer::TextureType::e2D,
				format,
				{ uint32_t( tex2D.extent().x ), uint32_t( tex2D.extent().y ), 1u },
				uint32_t( tex2D.levels() ),
				1u,
				renderer::SampleCountFlag::e1,
				renderer::ImageTiling::eOptimal,
				renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled
			}
			, renderer::MemoryPropertyFlag::eDeviceLocal );

		// Prepare copy regions
		std::vector< renderer::BufferImageCopy > bufferCopyRegions;
		uint32_t offset{ 0u };

		for ( uint32_t level = 0; level < tex2D.levels(); level++ )
		{
			renderer::BufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = renderer::ImageAspectFlag::eColour;
			bufferCopyRegion.imageSubresource.mipLevel = level;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0u;
			bufferCopyRegion.imageSubresource.layerCount = 1u;
			bufferCopyRegion.imageExtent.width = static_cast< uint32_t >( tex2D[level].extent().x );
			bufferCopyRegion.imageExtent.height = static_cast< uint32_t >( tex2D[level].extent().y );
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = offset;
			bufferCopyRegion.levelSize = tex2D[level].size();

			bufferCopyRegions.push_back( bufferCopyRegion );

			// Increase offset into staging buffer for next level / face
			offset += uint32_t( tex2D[level].size() );
		}

		auto cmdBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
		renderer::ImageSubresourceRange subresourceRange
		{
			renderer::ImageAspectFlag::eColour,
			0,
			uint32_t( tex2D.levels() ),
			0,
			1,
		};
		cmdBuffer->begin();

		// Memory barrier to transfer destination.
		cmdBuffer->memoryBarrier( renderer::PipelineStageFlag::eAllCommands
			, renderer::PipelineStageFlag::eAllCommands
			, renderer::ImageMemoryBarrier{ 0u
				, renderer::AccessFlag::eTransferWrite
				, renderer::ImageLayout::eUndefined
				, renderer::ImageLayout::eTransferDstOptimal
				, ~( 0u )
				, ~( 0u )
				, *m_texture
				, subresourceRange } );

		// Copy the cube map faces from the staging buffer to the optimal tiled image.
		cmdBuffer->copyToImage( bufferCopyRegions
			, *stagingBuffer
			, *m_texture );

		// Memory barrier to shader read.
		cmdBuffer->memoryBarrier( renderer::PipelineStageFlag::eAllCommands
			, renderer::PipelineStageFlag::eAllCommands
			, renderer::ImageMemoryBarrier{ renderer::AccessFlag::eTransferWrite
				, renderer::AccessFlag::eShaderRead
				, renderer::ImageLayout::eTransferDstOptimal
				, renderer::ImageLayout::eShaderReadOnlyOptimal
				, ~( 0u )
				, ~( 0u )
				, *m_texture
				, subresourceRange } );

		cmdBuffer->end();
		auto fence = m_device->createFence();
		m_device->getGraphicsQueue().submit( *cmdBuffer, fence.get() );
		fence->wait( renderer::FenceTimeout );

		// Create the sampler.
		m_sampler = m_device->createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear );

		// Create the texture view for shader read.
		m_view = m_texture->createView( renderer::TextureViewType::e2D
			, format
			, 0u
			, uint32_t( tex2D.levels() ) );
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment }
		};
		m_descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( 1u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
			, *m_view
			, *m_sampler );
		m_descriptorSet->update();
	}

	void RenderPanel::doCreateRenderPass()
	{
		renderer::AttachmentDescriptionArray attaches
		{
			{
				0u,
				m_swapChain->getFormat(),
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::ePresentSrc,
			}
		};
		renderer::AttachmentReferenceArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< renderer::RenderSubpass >( renderer::PipelineBindPoint::eGraphics
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
		m_renderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead }
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead } );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexBuffer = renderer::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_vertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_vertexLayout = renderer::makeLayout< TexturedVertexData >( 0 );
		m_vertexLayout->createAttribute( 0u
			, renderer::Format::eR32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_vertexLayout->createAttribute( 1u
			, renderer::Format::eR32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_vertexData
			, *m_vertexBuffer
			, renderer::PipelineStageFlag::eVertexInput );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( *m_device
			, 0u
			, 1000000u );
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = m_device->createPipelineLayout( *m_descriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.vert" )
			|| !wxFileExists( shadersFolder / "shader.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< renderer::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "shader.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "shader.frag" ) );

		m_pipeline = m_pipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_renderPass,
			renderer::VertexInputState::create( *m_vertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
			renderer::RasterisationState{},
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			{ renderer::DynamicState::eScissor, renderer::DynamicState::eViewport }
		} );
	}

	void RenderPanel::doPrepareFrames()
	{
		m_queryPool = m_device->createQueryPool( renderer::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffers = m_swapChain->createCommandBuffers();
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		auto dimensions = m_swapChain->getDimensions();

		for ( size_t i = 0u; i < m_commandBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
			{
				commandBuffer.resetQueryPool( *m_queryPool
					, 0u
					, 2u );
				commandBuffer.beginRenderPass( *m_renderPass
					, frameBuffer
					, { m_swapChain->getClearColour() }
					, renderer::SubpassContents::eInline );
				commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
					, *m_queryPool
					, 0u );
				commandBuffer.bindPipeline( *m_pipeline );
				commandBuffer.setViewport( { dimensions.width
					, dimensions.height
					, 0
					, 0 } );
				commandBuffer.setScissor( { 0
					, 0
					, dimensions.width
					, dimensions.height } );
				commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
				commandBuffer.bindDescriptorSet( *m_descriptorSet
					, *m_pipelineLayout );
				commandBuffer.draw( 4u );
				commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
					, *m_queryPool
					, 1u );
				commandBuffer.endRenderPass();

				auto res = commandBuffer.end();

				if ( !res )
				{
					std::stringstream stream;
					stream << "Command buffers recording failed.";
					throw std::runtime_error{ stream.str() };
				}
			}
		}
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			auto & queue = m_device->getGraphicsQueue();
			auto res = queue.submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources );

			renderer::UInt32Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, renderer::QueryResultFlag::eWait
				, values );
			// Elapsed time in nanoseconds
			auto elapsed = std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device->getTimestampPeriod() ) ) };
			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( elapsed )
				, std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
		}
	}

	void RenderPanel::doResetSwapChain()
	{
		doPrepareFrames();
	}

	void RenderPanel::onTimer( wxTimerEvent & event )
	{
		if ( event.GetId() == int( Ids::RenderTimer ) )
		{
			doDraw();
		}
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_timer->Stop();
		m_device->waitIdle();
		wxSize size{ GetClientSize() };
		m_swapChain->reset( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
