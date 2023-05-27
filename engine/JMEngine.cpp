#include "source/vulkan_rhi.h"

#include <memory>

int main(int, char **)
{
	JMEngine::WindowInfo windowInfo;
	std::shared_ptr<JMEngine::WindowSystem> window = std::make_shared<JMEngine::WindowSystem>();
	std::shared_ptr<JMEngine::VulkanRHI> kulkanRHI = std::make_shared<JMEngine::VulkanRHI>();
	window->Initialize(windowInfo);
	kulkanRHI->Initialize(window);
	GLFWwindow *mainWindow = window->GetWindow();

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();
		kulkanRHI->DrawFrame();
	}
	kulkanRHI->Clear();
	return 0;
}
