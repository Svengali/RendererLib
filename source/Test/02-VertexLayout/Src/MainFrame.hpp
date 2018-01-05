#pragma once

#include "Prerequisites.hpp"

#include <Renderer/Renderer.hpp>

#include <MainFrame.hpp>

namespace vkapp
{
	class MainFrame
		: public common::MainFrame
	{
	public:
		MainFrame( wxString const & rendererName );

	private:
		wxPanel * doCreatePanel( wxSize const & size, renderer::Renderer const & renderer )override;
	};
}