#include "GlPipeline.hpp"

#include "GlDevice.hpp"
#include "GlPipelineLayout.hpp"
#include "GlRenderingResources.hpp"
#include "GlRenderPass.hpp"
#include "GlShaderProgram.hpp"
#include "GlVertexLayout.hpp"

namespace gl_renderer
{
	Pipeline::Pipeline( renderer::Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::ShaderProgram const & program
		, renderer::VertexLayoutCRefArray const & vertexLayouts
		, renderer::RenderPass const & renderPass
		, renderer::PrimitiveTopology topology
		, renderer::RasterisationState const & rasterisationState
		, renderer::ColourBlendState const & colourBlendState )
		: renderer::Pipeline{ device
			, layout
			, program
			, vertexLayouts
			, renderPass
			, topology
			, rasterisationState
			, colourBlendState }
		, m_layout{ layout }
		, m_program{ program }
		, m_cbState{ colourBlendState }
		, m_rsState{ rasterisationState }
	{
	}

	renderer::Pipeline & Pipeline::finish()
	{
		return *this;
	}

	renderer::Pipeline & Pipeline::multisampleState( renderer::MultisampleState const & state )
	{
		m_msState = state;
		return *this;
	}

	renderer::Pipeline & Pipeline::depthStencilState( renderer::DepthStencilState const & state )
	{
		m_dsState = state;
		return *this;
	}

	renderer::Pipeline & Pipeline::tessellationState( renderer::TessellationState const & state )
	{
		m_tsState = state;
		return *this;
	}

	renderer::Pipeline & Pipeline::viewport( renderer::Viewport const & viewport )
	{
		m_viewport = viewport;
		return *this;
	}

	renderer::Pipeline & Pipeline::scissor( renderer::Scissor const & scissor )
	{
		m_scissor = scissor;
		return *this;
	}
}