/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Viewport.hpp"

namespace renderer
{
	Viewport::Viewport( uint32_t width
		, uint32_t height
		, int32_t x
		, int32_t y )
		: m_viewport{ width, height, x, y }
	{
	}
}
