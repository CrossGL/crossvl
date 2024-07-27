#pragma once
#include <Core/Common.h>
#include <Core/Application/Application.h>

namespace CGL
{
	CGL_DECLARE_LOG_CATEGORY(BlankApp);

	class BlankApp : public Core::Application
	{
	protected:
		using Super = Core::Application;

	public:
		BlankApp();
		~BlankApp();

	private:
		bool OnInit() override final;
		void OnUpdate() override final;
		void OnRender() override final;
		void OnResize(u32 width, u32 height) override final;
		void OnShutdown() override final;
	};
}