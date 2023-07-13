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
	m_ImGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	m_ImGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	m_ImGuiIO.ConfigFlags |= ImGuiWindowFlags_MenuBar;
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (m_ImGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init();

	return true;
}

void LASC::Window::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
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

		ImGui::Begin("Launcher");
		ImGui::Text("This is some useful text.");

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Metrics/Debugger", NULL, false);
			ImGui::MenuItem("Style Editor", NULL, false);
			ImGui::MenuItem("About Dear ImGui", NULL, false);
			ImGui::EndMenu();
		}

		ImGui::End();
		ImGui::Render();
		bool open = true;
		ImGui::ShowDemoWindow(&open);

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
