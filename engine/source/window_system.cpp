#include "source/window_system.h"
#include "source/global/macro.h"

namespace JMEngine
{
	WindowSystem::~WindowSystem()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void WindowSystem::Initialize(const WindowInfo &info)
	{
		if (!glfwInit())
		{
			LOG_FATAL("failed to initialize GLFW");
			return;
		}

		m_width = info.width;
		m_height = info.height;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(m_width, m_height, info.title, nullptr, nullptr);
		if (!m_window)
		{
			LOG_FATAL("failed to create window");
			glfwTerminate();
			return;
		}
	}
}
