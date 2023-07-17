#pragma once

#include <stdint.h>

namespace LASC {

	static const char* WindowResolutions[4] = { 
		"1280 x 720", "1920 x 1080", "2560 x 1440", "3840 x 2160"
	};

	struct LauncherOptions
	{
		bool UserClosedApplication;
		bool Fullscreen;
		uint32_t Width;
		uint32_t Height;
	};

}
