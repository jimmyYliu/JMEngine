#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace JMEngine
{
	struct WindowInfo
	{
		int width{ 1280 };
		int height{ 720 };
		const char* title{ "JMEngine" };
		bool isFullscreen{ false };
	};

	class WindowSystem final
	{
	public:
		~WindowSystem();
		void Initialize(const WindowInfo& info);
		inline GLFWwindow* GetWindow() const { return m_window; }
		// TODO: 主循环放到其它地方实现
		void Run();

	private:
		GLFWwindow* m_window{ nullptr };
		int m_width{ 0 };
		int m_height{ 0 };
	};
}