/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_FenceCreateFlag_HPP___
#define ___Renderer_FenceCreateFlag_HPP___
#pragma once

#include <Utils/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class FenceCreateFlag
		: uint32_t
	{
		eSignaled = 0x00000001,
	};
	Utils_ImplementFlag( FenceCreateFlag )
}

#endif