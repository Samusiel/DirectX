#include "stdafx.h"
#include "System/App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;

	auto& app = System::App::instance();

	// Initialize and run the system object.
	result = System::App::instance()->init();
	if (app)
	{
		app->run();
	}

	// Shutdown and release the system object.
	app->shutdown();

	return 0;
}