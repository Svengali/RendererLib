#pragma once

#include "Prerequisites.hpp"

#include <Renderer/Device.hpp>

#include <Utils/UtilsSignal.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	/**
	*@~french
	*@brief
	*	Le panneau de rendu.
	*@~english
	*@brief
	*	The render panel.
	*/
	class RenderPanel
		: public wxPanel
	{
	public:
		/**
		*@~french
		*@brief
		*	Constructeur.
		*	Initialise le device logique.
		*@param parent
		*	La fenêtre parente.
		*@param size
		*	La taille de la fenêtre.
		*@param instance
		*	L'instance Vulkan.
		*@~english
		*@brief
		*	Constructor.
		*	Initialises the logical device.
		*@param parent
		*	The parent window.
		*@param size
		*	The window dimensions.
		*@param instance
		*	The Vulkan instance.
		*/
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, renderer::Renderer const & vulkan );
		/**
		*@~french
		*@brief
		*	Destructeur.
		*@~english
		*@brief
		*	Destructor.
		*/
		~RenderPanel();

	private:
		/**
		*@~french
		*@brief
		*	Crée le device logique.
		*@~english
		*@brief
		*	Creates the logical device.
		*/
		void doCreateDevice( renderer::Renderer const & vulkan );

	private:
		renderer::DevicePtr m_device;
	};
}