/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Texture_HPP___
#define ___VkRenderer_Texture_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Image/Texture.hpp>

#include <Utils/Vec2.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
		: public renderer::Texture
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device );
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture();
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*/
		void generateMipmaps()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout général.
		*\param[in] accessFlags
		*	Les accès voulus, une fois que la transition est effectuée.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeGeneralLayout( renderer::ImageSubresourceRange const & range
			, renderer::AccessFlags accessFlags )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeTransferDestination( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeTransferSource( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeShaderInputResource( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\remarks
		*	Spécifique aux images prondeur et/ou stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilReadOnly( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeColourAttachment( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur/stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilAttachment( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de presentation.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makePresentSource( renderer::ImageSubresourceRange const & range )const override;
		/**
		*\return
		*	L'image OpenGL.
		*/
		inline GLuint getImage()const noexcept
		{
			assert( m_texture != GL_INVALID_INDEX );
			return m_texture;
		}

	private:
		renderer::ImageMemoryBarrier doMakeLayoutTransition( renderer::ImageLayout layout
			, uint32_t queueFamily
			, renderer::AccessFlags dstAccessMask
			, renderer::ImageSubresourceRange const & range )const;
		/**
		*\copydoc	renderer::Texture::doSetImage1D
		*/
		void doSetImage1D( renderer::ImageUsageFlags usageFlags
			, renderer::ImageTiling tiling
			, renderer::MemoryPropertyFlags memoryFlags )override;
		/**
		*\copydoc	renderer::Texture::doSetImage2D
		*/
		void doSetImage2D( renderer::ImageUsageFlags usageFlags
			, renderer::ImageTiling tiling
			, renderer::MemoryPropertyFlags memoryFlags )override;
		/**
		*\copydoc	renderer::Texture::doSetImage3D
		*/
		void doSetImage3D( renderer::ImageUsageFlags usageFlags
			, renderer::ImageTiling tiling
			, renderer::MemoryPropertyFlags memoryFlags )override;

	private:
		Device const & m_device;
		GlTextureType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
