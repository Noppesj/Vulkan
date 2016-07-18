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
		void ValidationLayers(VkInstanceCreateInfo& instanceInfo);
		void Win32_LoadVulkanExtensions();
		static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanReportCallback(VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objectType,
			uint64_t object,
			size_t location,
			int32_t messageCode,
			const char* pLayerPrefix,
			const char* pMessage,
			void* pUserData);
		
		PFN_vkCreateInstance vkCreateInstance = NULL;

		//DEBUG
		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;
		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = NULL;
		PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = NULL;
		PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = NULL;
		PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = NULL;
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;
		VkDebugReportCallbackEXT m_Callback;
		//END OF DEBUG

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
		void ReportExtensions();

		/*struct VulkanContext
		{
			uint32_t width;
			uint32_t heigt;

			VkInstance instance;
		};*/
	};
}