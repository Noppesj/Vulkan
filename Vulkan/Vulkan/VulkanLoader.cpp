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
	}	
}

void VulkanLoader::CreateInstance()
{
	VkApplicationInfo applicationInfo = {};

	applicationInfo.sType					= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext					= NULL;
	applicationInfo.pApplicationName		= "Test Vulcan";
	applicationInfo.pEngineName				= NULL;
	applicationInfo.engineVersion			= 1;
	applicationInfo.apiVersion				= VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo = {};

	instanceInfo.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo			= &applicationInfo;
	instanceInfo.enabledLayerCount			= 0;
	instanceInfo.ppEnabledLayerNames		= NULL;
	instanceInfo.enabledExtensionCount		= 0;
	instanceInfo.ppEnabledExtensionNames	= NULL;

	VkResult result = vkCreateInstance(&instanceInfo, NULL, &m_Instance);
	CheckVulkanResult(result, "Failed to create vulkan instance.");
}

void  VulkanLoader::ValidationLayers()
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
			OutputDebugStringA(layersAvailable[i].layerName);
			OutputDebugStringA("\n");
			if (strcmp(layersAvailable[i].layerName,"VK_LAYER_LUNARG_standard_validation") == 0)
			{
				foundValidation = true;
			}
		}

		assert(foundValidation && "Could not found validation layer.");
	}
}