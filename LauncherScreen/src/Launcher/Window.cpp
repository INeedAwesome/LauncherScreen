#include "lascpch.h"

#include "Window.h"

bool LASC::Window::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	// init window 
	if (!glfwInit())
		return false;
	
	m_Window = glfwCreateWindow(m_Width, m_Height, "Launcher", nullptr, nullptr);
	if (!m_Window) 
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_Window);

	// glad loader



	return true;
}

void LASC::Window::Shutdown()
{
	glfwTerminate();
}

void LASC::Window::Update()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void LASC::Window::CenterWindow()
{
}
