#pragma once

struct GLFWwindow;

class WindowDragging
{
public:
	static WindowDragging& Get() // Singleton is accessed via getInstance()
	{
		static WindowDragging instance; // lazy singleton, instantiated on first use
		return instance;
	}

	void Update(GLFWwindow* window, int imguiMainSection);

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

private:
	WindowDragging(void) // private constructor necessary to allow only 1 instance
	{
	}

	WindowDragging(WindowDragging const&); // prevent copies
	void operator=(WindowDragging const&); // prevent assignments

private:
	int m_LastPosX;
	int m_LastPosY;
	int m_PosX;
	int m_PosY;
	int m_DeltaX;
	int m_DeltaY;

	int32_t m_PositionX = 0;
	int32_t m_PositionY = 0;

	bool buttonEvent;

	int32_t m_ImguiMainSection;

};

