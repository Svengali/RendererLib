/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Renderer/CommandPool.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Encapsulation d'un vk::CommandPool.
	*/
	class CommandPool
		: public renderer::CommandPool
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\param[in] queueFamilyIndex
		*	L'index de la famille à laquelle appartient le pool.
		*\param[in] flags
		*	Combinaison binaire de VkCommandPoolCreateFlagBits.
		*/
		CommandPool( renderer::Device const & device
			, uint32_t queueFamilyIndex
			, renderer::CommandPoolCreateFlags flags = 0 );
		/**
		*\brief
		*	Crée un tampon de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*\return
		*	Le tampon de commandes créé.
		*/
		renderer::CommandBufferPtr createCommandBuffer( bool primary )const override;
	};
}
