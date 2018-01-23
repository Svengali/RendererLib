#include "RenderPass/GlRenderBuffer.hpp"

#include "Core/GlDevice.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

#include <Image/ImageSubresourceRange.hpp>

namespace gl_renderer
{
	namespace
	{
		renderer::ImageUsageFlag getAttachmentFlag( renderer::PixelFormat format )
		{
			switch ( format )
			{
			case renderer::PixelFormat::eL8:
			case renderer::PixelFormat::eL8A8:
			case renderer::PixelFormat::eR8G8B8:
			case renderer::PixelFormat::eRGB565:
			case renderer::PixelFormat::eR8G8B8A8:
			case renderer::PixelFormat::eB8G8R8A8:
			case renderer::PixelFormat::eRGBA5551:
			case renderer::PixelFormat::eRGBA4444:
				return renderer::ImageUsageFlag::eColourAttachment;

			case renderer::PixelFormat::eD16:
			case renderer::PixelFormat::eD24S8:
			case renderer::PixelFormat::eD32F:
			case renderer::PixelFormat::eS8:
				return renderer::ImageUsageFlag::eDepthStencilAttachment;

			default:
				assert( false && "Unsupported pixel format." );
				return renderer::ImageUsageFlag::eColourAttachment;
			}
		}
	}

	RenderBuffer::RenderBuffer( Device const & device
		, renderer::PixelFormat format
		, renderer::UIVec2 const & size )
		: renderer::RenderBuffer{ device, format, size }
		, m_texture{ device }
	{
		m_texture.setImage( format
			, size
			, getAttachmentFlag( format )
			, renderer::ImageTiling::eOptimal );
		makeDepthStencilAttachment();
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeColourAttachment()const
	{
		return m_texture.makeColourAttachment( { renderer::getAspectMask( m_texture.getFormat() )
			, 0u
			, 1u
			, 0u
			, 1u } );
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeDepthStencilAttachment()const
	{
		return m_texture.makeDepthStencilAttachment( { renderer::getAspectMask( m_texture.getFormat() )
			, 0u
			, 1u
			, 0u
			, 1u } );
	}
}
