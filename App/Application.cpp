#include "Application.h"
#include <Core/Graphics/Renderer.h>

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(VisualizerApp);

	Graphics::Renderer* renderer = nullptr;

	Visualizer::Visualizer()
	{
		CGL_LOG(VisualizerApp, Trace, "Created Visualizer App");
	}
	Visualizer::~Visualizer()
	{
		CGL_LOG(VisualizerApp, Trace, "Destroyed Visualizer App");
	}

	bool Visualizer::OnInit()
	{
		if (!Super::OnInit())
			return false;

		renderer = new Graphics::Renderer(m_window, Graphics::RHIType::DirectX11);

		CGL_LOG(VisualizerApp, Info, "Initialized Visualizer App");

		return true;
	}

	void Visualizer::OnUpdate()
	{
		Super::OnUpdate();
	}

	void Visualizer::OnRender()
	{
		Super::OnRender();
	}

	void Visualizer::OnResize(u32 width, u32 height)
	{
		CGL_LOG(VisualizerApp, Info, "Resized Visualizer App {}x{}", width, height);
		Super::OnResize(width, height);
	}

	void Visualizer::OnShutdown()
	{
		if (renderer)
		{
			delete renderer;
			renderer = nullptr;
		}
		CGL_LOG(VisualizerApp, Info, "Shutting down Visualizer App");
		Super::OnShutdown();
	}
}
