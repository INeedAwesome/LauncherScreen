#pragma once
#include "lascpch.h"

#include "GLFW/glfw3.h"

namespace LASC {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Launcher",
			unsigned int width = 550,
			unsigned int height = 300)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		Window() {}

		bool Init(uint32_t width = 550, uint32_t height = 300);
		void Shutdown();
		void Update();

	private:
		void CenterWindow(); 

	private: 
		GLFWwindow* m_Window = nullptr;
		
		WindowProps m_WindowProps;

		bool m_shouldClose = false;

		uint32_t m_width = 550;
		uint32_t m_height = 300;
	
	};
}
