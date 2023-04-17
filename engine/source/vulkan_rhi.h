#pragma once

#include "window_system.h"

#include <vector>
#include <memory>
#include <optional>

namespace JMEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> computeFamily;

		bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value(); }
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR        capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR>   presentModes;
	};

	class VulkanRHI final
	{
	public:
		void Initialize(std::shared_ptr<WindowSystem>& windowSystem);

		void Clear();

	private:
		const std::vector<char const*> m_validationLayers{ "VK_LAYER_KHRONOS_validation" };
		std::vector<char const*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		bool m_enableValidationLayers{ true };
		QueueFamilyIndices m_queueIndices;

		GLFWwindow* m_window{ nullptr };

		VkInstance m_instance{ nullptr };
		VkDebugUtilsMessengerEXT m_debugMessenger{ nullptr };
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
		VkDevice m_device{ nullptr };
		VkQueue m_graphicsQueue{ nullptr };

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);

	};
}