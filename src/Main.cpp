/*
	Ludum Dare 28 Entry
	Copyright (c) David Avedissian 2013
*/
#include "Common.h"
#include "App.h"

int main(int argc, char** argv)
{
	int exitFlag = EXIT_SUCCESS;

	try
	{
		App app;
		app.run();
	}
	catch (std::exception& e)
	{
		// Display a dialog box
		stringstream ss;
		ss << "An unhandled exception has been thrown!" << endl << endl << "Message: " << e.what();
		MessageBoxA(NULL, ss.str().c_str(), "Exception", MB_OK);
		exitFlag = EXIT_FAILURE;
	}

	return exitFlag;
}

// Win32 Main
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(__argc, __argv);
}