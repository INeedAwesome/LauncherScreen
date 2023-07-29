#include "lascpch.h"

#include "Window.h"

#include <glad/glad.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

LASC::Window::Window()
	: m_ImGuiIO()
{

}

bool LASC::Window::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	// init window 
	if (!glfwInit())
		return false;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Launcher", nullptr, nullptr);
	if (!m_Window) 
	{
		glfwTerminate();
		return false;
	}

	auto cursorPosCallback = [](GLFWwindow* window, double xpos, double ypos) 
	{
		WindowDragging::Get().cursor_position_callback(window, xpos, ypos);
	};
	auto mouseButtonCallback = [](GLFWwindow * window, int button, int action, int mods) 
	{
		WindowDragging::Get().mouse_button_callback(window, button, action, mods);
	};

	glfwSetCursorPosCallback(m_Window, cursorPosCallback);
	glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);

	glfwMakeContextCurrent(m_Window);

	// glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, m_Width, m_Height);

	ApplyStyles();

	CenterWindow();

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_ImGuiIO = ImGui::GetIO();
	(void)m_ImGuiIO;

	m_ImGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//m_ImGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	m_ImGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//m_ImGuiIO.ConfigFlags |= ImGuiWindowFlags_MenuBar;
	//io.ConfigViewportsNoAutoMerge = true;
	//m_ImGuiIO.ConfigViewportsNoTaskBarIcon = true;
	m_ImGuiIO.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\REFSAN.TTF", 22.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].x = 10.0f / 255.0f;
	style.Colors[ImGuiCol_WindowBg].y = 38.0f / 255.0f;
	style.Colors[ImGuiCol_WindowBg].z = 71.0f / 255.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	style.Colors[ImGuiCol_Button].x = 20.0f / 255.0f;
	style.Colors[ImGuiCol_Button].y = 66.0f / 255.0f;
	style.Colors[ImGuiCol_Button].z = 114.0f / 255.0f;
	style.Colors[ImGuiCol_Button].w = 1.0f;

	style.Colors[ImGuiCol_ButtonHovered].x = 32.0f / 255.0f;
	style.Colors[ImGuiCol_ButtonHovered].y = 82.0f / 255.0f;
	style.Colors[ImGuiCol_ButtonHovered].z = 149.0f / 255.0f;
	style.Colors[ImGuiCol_ButtonHovered].w = 1.0f;
	
	style.Colors[ImGuiCol_ButtonActive].x = 44.0f / 255.0f;
	style.Colors[ImGuiCol_ButtonActive].y = 116.0f / 255.0f;
	style.Colors[ImGuiCol_ButtonActive].z = 179.0f / 255.0f;
	style.Colors[ImGuiCol_ButtonActive].w = 1.0f;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init();

	return true;
}

LASC::LauncherOptions LASC::Window::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_Window);
	glfwTerminate();

	return m_LauncherOptions;
}

void LASC::Window::Update()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1);

	while (!glfwWindowShouldClose(m_Window))
	{		
		/* Poll for and process events */
		glfwPollEvents();

		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);

		WindowDragging::Get().Update(m_Window, m_MainSectionBeginY);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiRender();

		ImGui::End();
		ImGui::Render();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (m_ImGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);


	}
}

void LASC::Window::ApplyStyles()
{
	DWORD style = WS_POPUP | WS_VISIBLE;
	DWORD exStyle = WS_EX_APPWINDOW;

	HWND hwnd = glfwGetWin32Window(m_Window);

	SetWindowLongW(hwnd, GWL_STYLE, style);
	SetWindowLongW(hwnd, GWL_EXSTYLE, exStyle);
}

void LASC::Window::CenterWindow()
{
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor)
		return;

	const GLFWvidmode* desktop = glfwGetVideoMode(primaryMonitor);

	if (!desktop)
		return;

	glfwSetWindowPos(m_Window, (desktop->width - m_Width) / 2, (desktop->height - m_Height) / 2);
}

void LASC::Window::ImGuiRender()
{
	bool open = true;
	ImGui::Begin("Launcher", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImVec2 pos = { 0, 0 };
	ImGui::SetWindowPos("Launcher", pos);
	ImVec2 size = { (float)m_Width, (float)m_Height };
	ImGui::SetWindowSize("Launcher", size);

#pragma region GRAPHICS
	std::string graphicsText = "Graphics";
	float graphicsTextWidth = ImGui::CalcTextSize(graphicsText.c_str()).x;
	float graphicsTextHeight = ImGui::CalcTextSize(graphicsText.c_str()).y;
	{
		ImGui::SetCursorPosX((m_Width - graphicsTextWidth) / 2);
		ImGui::Text(graphicsText.c_str());
	}

	static const char* current_item = LASC::WindowResolutions[1];


	std::string resolutionText = "Resolution: ";
	float resolutionTextWidth = ImGui::CalcTextSize(resolutionText.c_str()).x;
	float comboWidth = 350.0f;
	ImGui::SetCursorPosX(((m_Width - resolutionTextWidth) / 2) - comboWidth/2);
	m_MainSectionBeginY = (graphicsTextHeight + 40.0f);
	ImGui::SetCursorPosY(m_MainSectionBeginY);
	{
		ImGui::Text(resolutionText.c_str());
		ImGui::SameLine();

		//ImGui::SetCursorPosX((m_Width - comboWidth) / 2);
		ImGui::SetNextItemWidth(comboWidth);

		if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(LASC::WindowResolutions); n++)
			{
				bool is_selected = (current_item == LASC::WindowResolutions[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(LASC::WindowResolutions[n], is_selected))
					current_item = LASC::WindowResolutions[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
	}

	std::string fullscreenText = "Fullscreen: ";
	float fullscreenTextWidth = ImGui::CalcTextSize(fullscreenText.c_str()).x;
	float checkboxWidth = 28;
	ImGui::SetCursorPosX(((m_Width - fullscreenTextWidth) / 2) - checkboxWidth);
	{
		ImGui::BeginGroup();
		ImGui::Text(fullscreenText.c_str());
		ImGui::SameLine();
		ImGui::Checkbox("##fullscreen", &m_LauncherOptions.Fullscreen);
		ImGui::EndGroup();
	}

	if (current_item == LASC::WindowResolutions[0])
	{
		m_LauncherOptions.Width = 1280;
		m_LauncherOptions.Height = 720;
	}
	else if (current_item == LASC::WindowResolutions[1])
	{
		m_LauncherOptions.Width = 1920;
		m_LauncherOptions.Height = 1080;
	}
	else if (current_item == LASC::WindowResolutions[2])
	{
		m_LauncherOptions.Width = 2560;
		m_LauncherOptions.Height = 1440;
	}
	else if (current_item == LASC::WindowResolutions[3])
	{
		m_LauncherOptions.Width = 3840;
		m_LauncherOptions.Height = 2160;
	}

#pragma endregion

#pragma region EXIT BUTTONS

	float px = m_Width / 2 - m_ButtonSize.x;
	float py = m_Height - m_ButtonSize.y - m_Padding;
	ImGui::SetCursorPos({ px , py });
	ImGui::BeginGroup();
	if (ImGui::Button("Play", m_ButtonSize)) 
	{
		m_LauncherOptions.UserClosedApplication = false;
		glfwSetWindowShouldClose(m_Window, true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel", m_ButtonSize)) 
	{
		m_LauncherOptions.UserClosedApplication = true;
		glfwSetWindowShouldClose(m_Window, true);
	}
	ImGui::EndGroup();
#pragma endregion
}
