#include <Launcher.h>

int main()
{
	std::cout << "Hello World\n";

	LASC::Window window;
	window.Init();
	window.Update();
	window.Shutdown();

	return 0;
}