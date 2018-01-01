/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Buffer_HPP___
#define ___VkRenderer_Buffer_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Renderer/Buffer.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class BufferBase
		: public renderer::BufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		BufferBase( renderer::Device const & device
			, uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags flags );
		/**
		*\brief
		*	Destructeur.
		*/
		~BufferBase();
		/**
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		uint8_t * lock( uint32_t offset
			, uint32_t size
			, renderer::MemoryMapFlags const & flags )const;
		/**
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*\param[in] modified
		*	Dit si le tampon a été modifié, et donc si la VRAM doit être mise à jour.
		*/
		void unlock( uint32_t size
			, bool modified )const;
		/**
		*\return
		*	Le tampon.
		*/
		inline GLuint getBuffer()const
		{
			assert( m_buffer != GL_INVALID_INDEX );
			return m_buffer;
		}

	private:
		GLuint m_buffer{ GL_INVALID_INDEX };
		GLenum m_target;
	};
}

#endif
