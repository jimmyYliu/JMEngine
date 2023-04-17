#include "vulkan_rhi.h"
#include "log_system.h"

#include <iostream>
#include <set>

namespace JMEngine
{
	void VulkanRHI::Initialize(std::shared_ptr<WindowSystem>& windowSystem)
	{
		m_window = windowSystem->GetWindow();

#ifdef NDEBUG
		m_enableValidationLayers = false;
#else
		LOG_DEBUG("enable validation layers in vulkan");
		m_enableValidationLayers = true;
#endif
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
	}

	void VulkanRHI::Clear()
	{
		if (m_enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		}
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}

	void VulkanRHI::CreateInstance()
	{
		if (m_enableValidationLayers && !CheckValidationLayerSupport())
		{
			LOG_ERROR("validation layers requested, but not available!");
		}

		// TODO:名称和版本可以调整为参数
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "JMEngineRenderer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "JMEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (m_enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
			createInfo.ppEnabledLayerNames = m_validationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create instance!");
		}
	}

	void VulkanRHI::SetupDebugMessenger()
	{
		if (!m_enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		{
			LOG_ERROR("failed to set up debug messenger!");
		}
	}

	void VulkanRHI::CreateSurface()
	{
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create window surface!");
		}
	}

	void VulkanRHI::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			LOG_ERROR("failed to find GPUs with Vulkan support!");
		}
		else
		{
			// find one device that matches our requirement
			// or find which is the best
			std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
			vkEnumeratePhysicalDevices(m_instance, &deviceCount, physicalDevices.data());

			std::vector<std::pair<int, VkPhysicalDevice>> rankedPhysicalDevices;
			for (const auto& device : physicalDevices)
			{
				VkPhysicalDeviceProperties physicalDeviceProperties;
				vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
				int score = 0;

				if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					score += 1000;
				}
				else if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
				{
					score += 100;
				}

				rankedPhysicalDevices.push_back({ score, device });
			}

			std::sort(rankedPhysicalDevices.begin(),
				rankedPhysicalDevices.end(),
				[](const std::pair<int, VkPhysicalDevice>& p1, const std::pair<int, VkPhysicalDevice>& p2)
				{
					return p1 > p2;
				});

			for (const auto& device : rankedPhysicalDevices)
			{
				if (IsDeviceSuitable(device.second))
				{
					m_physicalDevice = device.second;
					break;
				}
			}

			if (m_physicalDevice == VK_NULL_HANDLE)
			{
				LOG_ERROR("failed to find suitable physical device");
			}
		}
	}

	bool VulkanRHI::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VulkanRHI::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT,
		VkDebugUtilsMessageTypeFlagsEXT,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	void VulkanRHI::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VkResult VulkanRHI::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanRHI::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	bool VulkanRHI::IsDeviceSuitable(VkPhysicalDevice device)
	{
		auto queueIndices = FindQueueFamilies(device);
		bool isExtensionsSupported = CheckDeviceExtensionSupport(device);
		bool isSwapchainAdequate = false;
		if (isExtensionsSupported)
		{
			SwapChainSupportDetails swapchainSupportDetails = QuerySwapChainSupport(device);
			isSwapchainAdequate =
				!swapchainSupportDetails.formats.empty() && !swapchainSupportDetails.presentModes.empty();
		}

		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

		if (!queueIndices.IsComplete() || !isSwapchainAdequate || !physicalDeviceFeatures.samplerAnisotropy)
		{
			return false;
		}

		return true;
	}

	QueueFamilyIndices VulkanRHI::FindQueueFamilies(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				indices.computeFamily = i;
			}


			VkBool32 isPresentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,
				i,
				m_surface,
				&isPresentSupport);
			if (isPresentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}
			i++;
		}
		return indices;
	}

	SwapChainSupportDetails VulkanRHI::QuerySwapChainSupport(VkPhysicalDevice physicalDevice)
	{
		SwapChainSupportDetails detailsResult;

		// capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &detailsResult.capabilities);

		// formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			detailsResult.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(
				physicalDevice, m_surface, &formatCount, detailsResult.formats.data());
		}

		// present modes
		uint32_t presentmodeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentmodeCount, nullptr);
		if (presentmodeCount != 0)
		{
			detailsResult.presentModes.resize(presentmodeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(
				physicalDevice, m_surface, &presentmodeCount, detailsResult.presentModes.data());
		}

		return detailsResult;
	}

	bool VulkanRHI::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());
		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}
}