/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlCommandPool.hpp"

#include "GlCommandBuffer.hpp"
#include "GlDevice.hpp"

namespace gl_renderer
{
	CommandPool::CommandPool( renderer::Device const & device
		, uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags flags )
		: renderer::CommandPool{ device, queueFamilyIndex, flags }
	{
	}

	renderer::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
