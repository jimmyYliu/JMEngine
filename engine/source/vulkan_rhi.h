#pragma once

#include "source/window_system.h"

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
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanRHI final
	{
	public:
		void Initialize(std::shared_ptr<WindowSystem> &windowSystem);

		void Clear();
		void DrawFrame();
		void RecreateSwapchain();
		static void OnWindowResized(GLFWwindow *window, int width, int height);

	private:
		const std::vector<char const *> m_validationLayers{"VK_LAYER_KHRONOS_validation"};
		std::vector<char const *> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		bool m_enableValidationLayers{true};
		bool m_enablePointLightShadow{true};
		QueueFamilyIndices m_queueIndices;

		GLFWwindow *m_window{nullptr};

		VkInstance m_instance{nullptr};
		VkDebugUtilsMessengerEXT m_debugMessenger{nullptr};
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
		VkDevice m_device{nullptr};
		VkQueue m_presentQueue{nullptr};
		VkQueue m_graphicsQueue{nullptr};
		VkQueue m_computeQueue{nullptr};
		VkSwapchainKHR m_swapchain{nullptr};
		std::vector<VkImage> m_swapchainImages;
		VkFormat m_swapchainImageFormat;
		VkExtent2D m_swapchainExtent;
		std::vector<VkImageView> m_swapchainImageViews;
		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_graphicsPipeline;
		std::vector<VkFramebuffer> m_swapchainFramebuffers;
		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;
		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void CreateSemaphores();
		void CleanUpSwapchain();

		bool CheckValidationLayerSupport();
		std::vector<const char *> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
		VkShaderModule CreateShaderModule(const std::vector<char> &code);
		VkPhysicalDeviceFeatures GetRequiredPhysicalDeviceFeatures();

		// function pointers
		PFN_vkWaitForFences _vkWaitForFences;
		PFN_vkResetFences _vkResetFences;
		PFN_vkResetCommandPool _vkResetCommandPool;
		PFN_vkBeginCommandBuffer _vkBeginCommandBuffer;
		PFN_vkEndCommandBuffer _vkEndCommandBuffer;
		PFN_vkCmdBeginRenderPass _vkCmdBeginRenderPass;
		PFN_vkCmdNextSubpass _vkCmdNextSubpass;
		PFN_vkCmdEndRenderPass _vkCmdEndRenderPass;
		PFN_vkCmdBindPipeline _vkCmdBindPipeline;
		PFN_vkCmdSetViewport _vkCmdSetViewport;
		PFN_vkCmdSetScissor _vkCmdSetScissor;
		PFN_vkCmdBindVertexBuffers _vkCmdBindVertexBuffers;
		PFN_vkCmdBindIndexBuffer _vkCmdBindIndexBuffer;
		PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
		PFN_vkCmdDrawIndexed _vkCmdDrawIndexed;
		PFN_vkCmdClearAttachments _vkCmdClearAttachments;
	};
}