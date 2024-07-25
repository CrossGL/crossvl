#include "HelloTriangleApp/HelloTriangleApp.h"
#include <Core/Utils/Singleton.h>
#include <Core/Platform/LogOutput.h>
#include <Core/Logging/LogManager.h>

int main()
{
	using namespace CGL::Core;

	auto& logManager = Utils::Singleton<LogManager>::Get();
	logManager.AttachOutput<ConsoleLogOutput>("ConsoleLogOutput", "CGL Log");
	logManager.AttachOutput<FileLogOutput>("FileLogOutput", "HelloTriangleSampleLog.txt");

	{
		CGL::HelloTriangleApp app;
		app.Run();
	}

	Utils::Singleton<LogManager>::Destroy();

	return 0;
}
