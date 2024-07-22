#pragma once
#include <Core/Common.h>
#include <Core/Application/Application.h>

namespace CGL
{
	CGL_DECLARE_LOG_CATEGORY(VisualizerApp);

	class Visualizer : public Core::Application
	{
	protected:
		using Super = Core::Application;

	public:
		Visualizer();
		~Visualizer();

	private:
		bool OnInit() override final;
		void OnUpdate() override final;
		void OnRender() override final;
		void OnResize(u32 width, u32 height) override final;
		void OnShutdown() override final;
	};
}