/**
*\file
*	RenderWindow.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderWindow_HPP___
#define ___RenderLib_RenderWindow_HPP___
#pragma once

#include "CameraState.h"
#include "Debug.h"
#include "FontTexture.h"
#include "OverlayRenderer.h"
#include "RenderTarget.h"
#include "Scene.h"

#include <Renderer/Attribute.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/OpenGL.h>
#include <Renderer/Pipeline.hpp>

namespace render
{
	/**
	*\brief
	*	Classe de fenêtre de rendu.
	*\remarks
	*	Utilise une RenderTarget pour faire un rendu en texture,
	*	puis dessine le résultat dans la fenêtre,
	*	enfin, dessine les incrustations.
	*/
	class RenderWindow
	{
	public:
		/**
		*\brief
		*	Un sommet pour le rendu dans la fenêtre.
		*/
		struct Vertex
		{
			//! Une position en 2D.
			renderer::Vec2 position;
			//! Les coordonnées de texture.
			renderer::Vec2 texture;
		};

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] dimensions
		*	Les dimensions de la texture recevant le rendu.
		*\param[in] debug
		*	\p true pour activer les informations de débogage.
		*\param[in] loader
		*	Le loader de police.
		*/
		RenderWindow( renderer::IVec2 const & dimensions
			, render::FontLoader & loader
			, bool debug );
		/**
		*\brief
		*	Démarre le dessin d'une image.
		*/
		void beginFrame();
		/**
		*\brief
		*	Met à jour la scène.
		*/
		void update();
		/**
		*\brief
		*	Met à jour les incrustations de la scène.
		*/
		void updateOverlays();
		/**
		*\brief
		*	Dessine la scène.
		*\remarks
		*	Le dessin se fait dans la cible de rendu, puis les effets éventuels
		*	sont appliqués et enfin la cible de rendu est dessinée dans le
		*	tampon de la fenêtre.
		*/
		void draw()const noexcept;
		/**
		*\brief
		*	Termine le dessin d'une image.
		*/
		void endFrame();
		/**
		*\brief
		*	Redimensionne le viewport.
		*\param[in] size
		*	Les dimensions du viewport.
		*/
		void resize( renderer::IVec2 const & size )noexcept;
		/**
		*\return
		*	La scène.
		*/
		inline Scene const & scene()const noexcept
		{
			return m_scene;
		}
		/**
		*\return
		*	La scène.
		*/
		inline Scene & scene()noexcept
		{
			return m_scene;
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport const & viewport()const noexcept
		{
			return m_scene.viewport();
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport & viewport()noexcept
		{
			return m_scene.viewport();
		}
		/**
		*\return
		*	L'instance de picking.
		*/
		inline Picking const & picking()const noexcept
		{
			return m_picking;
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState const & state()const noexcept
		{
			return m_scene.state();
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState & state()noexcept
		{
			return m_scene.state();
		}
		/**
		*\brief
		*	Active le picking pour la prochaine frame.
		*\param[in] position
		*	La position de la souris.
		*/
		inline void pick( renderer::IVec2 const & position )
		{
			m_pickPosition = position;
			m_pick = true;
		}

	private:
		/**
		*\brief
		*	Dessine une texture dans le backbuffer.
		*/
		void doRenderTextureToScreen( renderer::Texture const & texture )const noexcept;

	private:
		renderer::RenderingResources m_resources;
		//! Le pipeline de rendu dans la fenêtre.
		renderer::Pipeline m_pipeline;
		//! La cible de rendu.
		RenderTarget m_target;
		//! La scène qui sera dessinée.
		Scene m_scene;
		//! Les dimensions de la fenêtre.
		renderer::IVec2 m_size;
		//! L'échantillonneur de la texture de la cible de rendu.
		renderer::SamplerPtr m_sampler;
		//! Le programme shader utilisé pour le rendu dans la fenêtre.
		renderer::ShaderProgramPtr m_program;
		//! Le tampon GPU contenant les sommets du rendu dans la fenêtre.
		renderer::VertexBufferPtr< Vertex > m_vbo;
		//! La variable uniforme contenant la texture de la cible.
		renderer::IntUniformPtr m_texUniform;
		//! Le viewport du rendu dans la fenêtre.
		Viewport m_viewport;
		//! Le renderer d'incrustations
		OverlayRendererPtr m_overlayRenderer;
		//! La position voulue pour le picking.
		renderer::IVec2 m_pickPosition;
		//! L'instance de picking.
		Picking m_picking;
		//! Dit si on doit exécuter le picking lors du dessin de la prochaine frame.
		mutable bool m_pick{ false };
		//! Les informations de débogage.
		Debug m_debug;
	};
}

#endif
