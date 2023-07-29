#include <Launcher.h>

int Main()
{
	std::cout << "Hello World\n";

	LASC::Window window;
	LASC::LauncherOptions options;
	window.Init(650, 350);
	window.Update();
	options = window.Shutdown();

	if (options.UserClosedApplication)
	{
		printf("Quitting!");
		return 0;
	}

	// continue on with application

	printf("Fullscreen: %d, Width %d, Height %d", options.Fullscreen, options.Width, options.Height);

	return 0;
}

#ifdef SB_DIST
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Main();
}
#endif
#ifndef SB_DIST
int main() 
{
	Main();
}
#endif // !LS_DIST