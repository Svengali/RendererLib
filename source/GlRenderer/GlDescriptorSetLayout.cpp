#include "GlDescriptorSetLayout.hpp"

#include "GlDescriptorSetPool.hpp"

namespace gl_renderer
{
	DescriptorSetLayout::DescriptorSetLayout( renderer::Device const & device
		, renderer::DescriptorSetLayoutBindingArray && bindings )
		: renderer::DescriptorSetLayout{ device, std::move( bindings ) }
	{
	}

	renderer::DescriptorSetPoolPtr DescriptorSetLayout::createPool( uint32_t maxSets )const
	{
		return std::make_unique< DescriptorSetPool >( *this, maxSets );
	}
}
