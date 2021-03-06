/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::QueryControlFlags en VkQueryControlFlags.
	*\param[in] flags
	*	Le renderer::QueryControlFlags.
	*\return
	*	Le VkQueryControlFlags.
	*/
	VkQueryControlFlags convert( renderer::QueryControlFlags const & flags );
}
