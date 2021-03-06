/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/CommandBuffer.hpp"

#include "Buffer/VertexBuffer.hpp"
#include "Buffer/UniformBuffer.hpp"
#include "Image/ImageSubresourceRange.hpp"
#include "Image/Texture.hpp"
#include "Image/TextureView.hpp"

namespace renderer
{
	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, bool primary )
	{
	}

	void CommandBuffer::bindVertexBuffer( uint32_t binding
		, BufferBase const & buffer
		, uint64_t offset )const
	{
		bindVertexBuffers( binding
			, BufferCRefArray{ buffer }
			, UInt64Array{ offset } );
	}

	void CommandBuffer::copyToImage( BufferImageCopy const & copyInfo
		, BufferBase const & src
		, Texture const & dst )const
	{
		copyToImage( BufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyToBuffer( BufferImageCopy const & copyInfo
		, Texture const & src
		, BufferBase const & dst )const
	{
		copyToBuffer( BufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src, dst );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src, dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( VertexBufferBase const & src
		, BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst );
	}

	void CommandBuffer::copyBuffer( VertexBufferBase const & src
		, VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src, dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( UniformBufferBase const & src
		, BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst );
	}

	void CommandBuffer::copyBuffer( UniformBufferBase const & src
		, UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst.getBuffer() );
	}

	void CommandBuffer::copyImage( TextureView const & src
		, TextureView const & dst )const
	{
		auto const & srcRange = src.getSubResourceRange();
		auto const & dstRange = dst.getSubResourceRange();
		copyImage( ImageCopy
			{
				{                                                   // srcSubresource
					getAspectMask( src.getFormat() ),
					srcRange.baseMipLevel,
					srcRange.baseArrayLayer,
					srcRange.layerCount
				},
				Offset3D{                                              // srcOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				{                                                   // dstSubresource
					getAspectMask( dst.getFormat() ),
					dstRange.baseMipLevel,
					dstRange.baseArrayLayer,
					dstRange.layerCount
				},
				Offset3D{                                              // dstOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				dst.getTexture().getDimensions()                    // extent
			}
			, src.getTexture()
			, ImageLayout::eTransferSrcOptimal
			, dst.getTexture()
			, ImageLayout::eTransferDstOptimal );
	}
}
