#include "BlankApp.h"

namespace CGL
{
	CGL_DEFINE_LOG_CATEGORY(BlankApp);

	BlankApp::BlankApp()
	{
		CGL_LOG(BlankApp, Trace, "Created Blank App");
	}

	BlankApp::~BlankApp()
	{
		CGL_LOG(BlankApp, Trace, "Destroyed Blank App");
	}

	bool BlankApp::OnInit()
	{
		if (!Super::OnInit())
			return false;

		CGL_LOG(BlankApp, Info, "Initialized Blank App");

		return true;
	}

	void BlankApp::OnUpdate()
	{
		// Update app here
	}

	void BlankApp::OnRender()
	{
		// Add render code here
	}

	void BlankApp::OnResize(u32 width, u32 height)
	{
		Super::OnResize(width, height);
	}

	void BlankApp::OnShutdown()
	{
		CGL_LOG(BlankApp, Info, "Shutting down Blank App");
		Super::OnShutdown();
	}
}
