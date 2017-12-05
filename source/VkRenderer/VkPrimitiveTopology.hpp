/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/PrimitiveTopology.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::PrimitiveTopology en VkPrimitiveTopology.
	*\param[in] format
	*	Le renderer::PrimitiveTopology.
	*\return
	*	Le VkPrimitiveTopology.
	*/
	VkPrimitiveTopology convert( renderer::PrimitiveTopology const & topology );
}