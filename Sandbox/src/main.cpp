#include <Launcher.h>

#ifdef SB_DIST
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
#ifndef SB_DIST
int main()
#endif // !LS_DIST
{
	std::cout << "Hello World\n";

	LASC::Window window;
	LASC::LauncherOptions options;
	window.Init();
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