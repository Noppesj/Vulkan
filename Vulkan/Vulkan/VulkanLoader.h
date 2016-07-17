#pragma once
#include "vulkan.h"
#include <Windows.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPES

namespace NP16
{
	class VulkanLoader
	{
	private:
		void CheckVulkanResult(VkResult &result, const char *msg);

		PFN_vkCreateInstance vkCreateInstance = NULL;
		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;
		
		bool m_UseDebug = true;

		//Todo: Remove this to struct
		uint32_t m_Width;
		uint32_t m_Heigt;

		VkInstance m_Instance;

	public:
		VulkanLoader();
		virtual ~VulkanLoader();

		void Win32_LoadVulkan();
		void CreateInstance();
		void ValidationLayers();
	};

	/*struct VulkanContext
	{
		uint32_t width;
		uint32_t heigt;

		VkInstance instance;
	};*/
}