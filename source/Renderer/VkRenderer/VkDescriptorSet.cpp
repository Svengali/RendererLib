#include "VkDescriptorSet.hpp"

#include "VkBuffer.hpp"
#include "VkBufferView.hpp"
#include "VkDescriptorSetBinding.hpp"
#include "VkDescriptorSetLayoutBinding.hpp"
#include "VkDescriptorSetLayout.hpp"
#include "VkDescriptorSetPool.hpp"
#include "VkSampler.hpp"
#include "VkTextureView.hpp"
#include "VkUniformBuffer.hpp"

namespace vk_renderer
{
	DescriptorSet::DescriptorSet( Device const & device
		, DescriptorSetPool const & pool )
		: renderer::DescriptorSet{ pool }
		, m_device{ device }
		, m_pool{ pool }
		, m_layout{ static_cast< DescriptorSetLayout const & >( pool.getLayout() ) }
	{
		auto layouts = makeVkArray< VkDescriptorSetLayout >( DescriptorSetLayoutCRefArray{ m_layout } );
		VkDescriptorSetAllocateInfo allocateInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, // sType
			nullptr,                                        // pNext
			pool,                                           // descriptorPool
			static_cast< uint32_t >( layouts.size() ),      // descriptorSetCount
			layouts.data()                                  // pSetLayouts
		};
		DEBUG_DUMP( allocateInfo );
		auto res = AllocateDescriptorSets( m_device
			, &allocateInfo
			, &m_descriptorSet );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Descriptor set allocation failed: " + getLastError() };
		}
	}

	DescriptorSet::~DescriptorSet()
	{
		m_bindings.clear();

		if ( !m_pool.hasAutomaticFree() )
		{
			FreeDescriptorSets( m_device
				, m_pool
				, 1u
				, &m_descriptorSet );
		}
	}

	renderer::CombinedTextureSamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::Sampler const & sampler )
	{
		m_bindings.emplace_back( std::make_unique< CombinedTextureSamplerBinding >( layoutBinding
			, *this
			, static_cast< TextureView const & >( view )
			, static_cast< Sampler const & >( sampler ) ) );
		return static_cast< CombinedTextureSamplerBinding const & >( *m_bindings.back() );
	}

	renderer::SamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Sampler const & sampler )
	{
		m_bindings.emplace_back( std::make_unique< SamplerBinding >( layoutBinding
			, *this
			, static_cast< Sampler const & >( sampler ) ) );
		return static_cast< SamplerBinding const & >( *m_bindings.back() );
	}

	renderer::SampledTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::ImageLayout layout )
	{
		m_bindings.emplace_back( std::make_unique< SampledTextureBinding >( layoutBinding
			, *this
			, static_cast< TextureView const & >( view )
			, layout ) );
		return static_cast< SampledTextureBinding const & >( *m_bindings.back() );
	}

	renderer::StorageTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view )
	{
		m_bindings.emplace_back( std::make_unique< StorageTextureBinding >( layoutBinding
			, *this
			, static_cast< TextureView const & >( view ) ) );
		return static_cast< StorageTextureBinding const & >( *m_bindings.back() );
	}

	renderer::UniformBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset )
	{
		offset = uniformBuffer.getOffset( offset );
		m_bindings.emplace_back( std::make_unique< UniformBufferBinding >( layoutBinding
			, *this
			, static_cast< UniformBuffer const & >( uniformBuffer )
			, offset ) );
		return static_cast< UniformBufferBinding const & >( *m_bindings.back() );
	}

	renderer::StorageBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, uint32_t offset )
	{
		m_bindings.emplace_back( std::make_unique< StorageBufferBinding >( layoutBinding
			, *this
			, static_cast< Buffer const & >( storageBuffer )
			, offset ) );
		return static_cast< StorageBufferBinding const & >( *m_bindings.back() );
	}

	renderer::UniformTexelBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, renderer::BufferView const & view )
	{
		m_bindings.emplace_back( std::make_unique< UniformTexelBufferBinding >( layoutBinding
			, *this
			, static_cast< UniformBuffer const & >( uniformBuffer )
			, static_cast< BufferView const & >( view ) ) );
		return static_cast< UniformTexelBufferBinding const & >( *m_bindings.back() );
	}

	renderer::StorageTexelBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, renderer::BufferView const & view )
	{
		m_bindings.emplace_back( std::make_unique< StorageTexelBufferBinding >( layoutBinding
			, *this
			, static_cast< Buffer const & >( storageBuffer )
			, static_cast< BufferView const & >( view ) ) );
		return static_cast< StorageTexelBufferBinding const & >( *m_bindings.back() );
	}

	void DescriptorSet::update()const
	{
		auto bindings = makeVkArray < VkWriteDescriptorSet >( m_bindings );
		DEBUG_DUMP( bindings );
		UpdateDescriptorSets( m_device
			, static_cast< uint32_t >( bindings.size() )
			, bindings.data()
			, 0
			, nullptr );
	}
}
