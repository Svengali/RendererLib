#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName = wxT( "10-SpinningCube" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
