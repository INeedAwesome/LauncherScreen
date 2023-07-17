#pragma once
#include "lascpch.h"

#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Options.h"

namespace LASC {

	class Window
	{
	public:
		Window() 
			: m_ImGuiIO() {}

		bool Init(uint32_t width = 550, uint32_t height = 300);
		LASC::LauncherOptions Shutdown();
		void Update();

	private:
		void ApplyStyles();
		void CenterWindow(); 

	private: 
		GLFWwindow* m_Window = nullptr;

		uint32_t m_Width = 550;
		uint32_t m_Height = 300;

		DWORD Style = WS_POPUP | WS_VISIBLE; //(border and everything)WS_OVERLAPPEDWINDOW | WS_VISIBLE / (no nothing)WS_POPUP | WS_VISIBLE
		bool m_ShouldClose = false;
		
		// pass this out of here to the main application
		LASC::LauncherOptions m_LauncherOptions { };

	private: // imgui variables
		ImGuiIO m_ImGuiIO;

		ImVec2 m_ButtonSize = { 130, 35 };
		float m_Padding = 12;
	};
}
