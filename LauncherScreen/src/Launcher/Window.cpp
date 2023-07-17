#include "lascpch.h"

#include "Window.h"

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
	style.Colors[ImGuiCol_WindowBg].x = 0.05f;
	style.Colors[ImGuiCol_WindowBg].y = 0.05f;
	style.Colors[ImGuiCol_WindowBg].z = 0.05f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool open = true;
		ImGui::Begin("Launcher", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

		ImVec2 pos = {0, 0};
		ImGui::SetWindowPos("Launcher", pos);
		ImVec2 size = { (float)m_Width, (float)m_Height };
		ImGui::SetWindowSize("Launcher", size);

#pragma region GRAPHICS

		ImGui::Text("Graphics");
				
		static const char* current_item = LASC::WindowResolutions[1];

		if (ImGui::BeginCombo(" dwd combo", current_item)) // The second parameter is the label previewed before opening the combo.
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

		ImGui::Checkbox("Fullscreen", &m_LauncherOptions.Fullscreen);

		if (!m_LauncherOptions.Fullscreen)
		{
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
		}

#pragma endregion
		
#pragma region EXIT BUTTONS

		float px = m_Width / 2 - m_ButtonSize.x;
		float py = m_Height - m_ButtonSize.y - m_Padding;
		ImGui::SetCursorPos({ px , py });
		ImGui::BeginGroup();
		ImGui::Button("Play", m_ButtonSize);
		ImGui::SameLine();
		if (ImGui::Button("Quit", m_ButtonSize))
			glfwSetWindowShouldClose(m_Window, true);
		ImGui::EndGroup();


#pragma endregion

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
