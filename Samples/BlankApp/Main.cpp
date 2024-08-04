#include "BlankApp/BlankApp.h"
#include <Core/Utils/Singleton.h>
#include <Core/Platform/LogOutput.h>
#include <Core/Logging/LogManager.h>

int main(int argc, char** argv)
{
	using namespace CGL::Core;

	auto& logManager = Utils::Singleton<LogManager>::Get();
	logManager.AttachOutput<ConsoleLogOutput>("ConsoleLogOutput", "CGL Log");
	logManager.AttachOutput<FileLogOutput>("FileLogOutput", "BlankAppSampleLog.txt");

	{
		CGL::BlankApp app(argc, argv);
		app.Run();
	}

	Utils::Singleton<LogManager>::Destroy();

	return 0;
}
