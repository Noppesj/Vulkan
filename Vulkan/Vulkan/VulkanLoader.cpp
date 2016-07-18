#include "VulkanLoader.h"
#include <assert.h>
#include <stdio.h>
#include <string>

using namespace NP16;

VulkanLoader::VulkanLoader()
{
#ifdef NDEBUG
	m_UseDebug = false;
#else
	m_UseDebug = true;

#endif
}

VulkanLoader::~VulkanLoader()
{
}

void VulkanLoader::CheckVulkanResult(VkResult &result, const char *msg)
{
	assert(result == VK_SUCCESS && msg);
}

void VulkanLoader::Win32_LoadVulkan()
{
	HMODULE vulkan_module = LoadLibrary(L"vulkan-1.dll");
	assert(vulkan_module != nullptr && "Failed to load vulkan dll-file.");

	vkCreateInstance = (PFN_vkCreateInstance)GetProcAddress(vulkan_module, "vkCreateInstance");

	assert(vkCreateInstance != nullptr && "Failed to create instance of GetProcAddress");

	if (m_UseDebug)
	{
		vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)GetProcAddress(
			vulkan_module, "vkEnumerateInstanceLayerProperties");

		vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)GetProcAddress(
			vulkan_module, "vkEnumerateInstanceExtensionProperties");

		vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkan_module, "vkGetInstanceProcAddr");
	}
}

void VulkanLoader::CreateInstance()
{
	VkApplicationInfo applicationInfo = {};

	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = NULL;
	applicationInfo.pApplicationName = "Test Vulcan";
	applicationInfo.pEngineName = NULL;
	applicationInfo.engineVersion = 1;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo = {};

	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &applicationInfo;
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = NULL;
	instanceInfo.enabledExtensionCount = 0;
	instanceInfo.ppEnabledExtensionNames = NULL;

	VkResult result = vkCreateInstance(&instanceInfo, NULL, &m_Instance);
	CheckVulkanResult(result, "Failed to create vulkan instance.");

	ValidationLayers(instanceInfo);
}

void VulkanLoader::ValidationLayers(VkInstanceCreateInfo& instanceInfo)
{
	if (m_UseDebug)
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, NULL);

		assert(layerCount != 0 && "Failed to find any Vulkan-layer in your system.");

		VkLayerProperties *layersAvailable = new VkLayerProperties[layerCount];
		vkEnumerateInstanceLayerProperties(&layerCount, layersAvailable);

		bool foundValidation = false;

		for (unsigned int i = 0; i < layerCount; ++i)
		{
			//OutputDebugStringA(layersAvailable[i].layerName);
			//OutputDebugStringA("\n");
			if (strcmp(layersAvailable[i].layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
			{
				foundValidation = true;
			}
		}

		assert(foundValidation && "Could not found validation layer.");

		const char *layers[] = { "VK_LAYER_LUNARG_standard_validation" };

		instanceInfo.enabledLayerCount = 1;
		instanceInfo.ppEnabledLayerNames = layers;
	}
}

void VulkanLoader::ReportExtensions()
{
	if (m_UseDebug)
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

		assert(extensionCount > 0 && "Failed to found any extension properties");

		VkExtensionProperties *extensionAvailable = new VkExtensionProperties[extensionCount];
		vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionAvailable);

		const char *extensions[] = { "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_debug_report" };
		uint32_t numberRequiredExtensions = sizeof(extensions) / sizeof(char*);
		uint32_t foundExtensions = 0;

		for (uint32_t i = 0; i < extensionCount; ++i)
		{
			for (uint32_t j = 0; j < numberRequiredExtensions; ++j)
			{
				if (strcmp(extensionAvailable[i].extensionName, extensions[j]) == 0)
				{
					foundExtensions++;
				}
			}
		}

		assert(foundExtensions == extensionCount && "Could not find debug extensions");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLoader::VulkanReportCallback(VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* pUserData)
{
	OutputDebugStringA(pLayerPrefix);
	OutputDebugStringA(" ");
	OutputDebugStringA(pMessage);
	OutputDebugStringA("\n");
	return VK_FALSE;
}

void VulkanLoader::Win32_LoadVulkanExtensions()
{
	if (m_UseDebug)
	{
		*(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(
			m_Instance, "vkCreateDebugReportCallbackEXT");

		*(void **)&vkDestroyDebugReportCallbackEXT = vkGetInstanceProcAddr(
			m_Instance, "vkDestroyDebugReportCallbackEXT");

		*(void **)&vkDebugReportMessageEXT = vkGetInstanceProcAddr(
			m_Instance, "vkDebugReportMessageEXT");

		VkDebugReportCallbackCreateInfoEXT callBackCreateInfo = {};

		callBackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		callBackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
			VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;

		callBackCreateInfo.pfnCallback = &VulkanReportCallback;
		callBackCreateInfo.pUserData = NULL;

		VkResult result = vkCreateDebugReportCallbackEXT(m_Instance,
			&callBackCreateInfo,
			NULL,
			&m_Callback);

		CheckVulkanResult(result, "Failed to create vulkan instance.");
	}
}
