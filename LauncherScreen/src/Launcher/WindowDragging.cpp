#include "lascpch.h"
#include "WindowDragging.h"

#include <GLFW/glfw3.h>

void WindowDragging::Update(GLFWwindow* window, int imguiMainSection)
{
	m_ImguiMainSection = imguiMainSection;

	if (buttonEvent) {

		if (m_PosY <= m_ImguiMainSection)
		{
			int32_t windowPosX, windowPosY;
			glfwGetWindowPos(window, &windowPosX, &windowPosY);

			glfwSetWindowPos(window, windowPosX + m_DeltaX, windowPosY + m_DeltaY);
			m_DeltaX = 0;
			m_DeltaY = 0;
			m_LastPosX += m_DeltaX;
			m_LastPosY += m_DeltaY;


		}

	}

}

void WindowDragging::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (buttonEvent) {
		if (ypos < m_ImguiMainSection)
		{
			m_DeltaX = xpos - m_LastPosX;
			m_DeltaY = ypos - m_LastPosY;
		}
	}
	m_PosX = xpos;
	m_PosY = ypos;
}

void WindowDragging::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		buttonEvent = true;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		m_LastPosX = floor(x);
		m_LastPosY = floor(y);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		buttonEvent = false;
		m_LastPosX = 0;
		m_LastPosY = 0;
	}
}
