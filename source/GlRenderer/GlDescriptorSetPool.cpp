#include "GlDescriptorSetPool.hpp"

#include "GlDescriptorSet.hpp"

namespace gl_renderer
{
	DescriptorSetPool::DescriptorSetPool( renderer::DescriptorSetLayout const & layout
		, uint32_t maxSets )
		: renderer::DescriptorSetPool{ layout, maxSets }
	{
	}

	renderer::DescriptorSetPtr DescriptorSetPool::createDescriptorSet()const
	{
		return std::make_unique< DescriptorSet >( *this );
	}
}
