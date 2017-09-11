#include "me/me.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		me::core::App* app = new me::core::App();
		app->create();
		app->run();
		
		if (app) delete app;
	}
	catch (me::util::RuntimeError e)
	{
		e.messageBox();
		return -1;
	}

	return 0;
}