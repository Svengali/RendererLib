/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Utils/PixelFormat.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un utils::PixelFormat en VkFormat.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	Le VkFormat.
	*/
	GLenum convert( utils::PixelFormat const & format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en utils::PixelFormat.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le utils::PixelFormat.
	*/
	utils::PixelFormat convert( GLenum format )noexcept;
}
