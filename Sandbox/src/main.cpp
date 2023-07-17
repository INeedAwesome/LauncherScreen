#include <Launcher.h>

int main()
{
	std::cout << "Hello World\n";

	LASC::Window window;
	LASC::LauncherOptions options;
	window.Init();
	if (window.Update()) // if user clicked Quit
	{
		printf("Quitting!");
		return 0;
	}
	options = window.Shutdown();

	printf("Fullscreen: %d, Width %d, Height %d", options.Fullscreen, options.Width, options.Height);

	return 0;
}