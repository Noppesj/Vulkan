#include "Display.h"
#include "VulkanLoader.h"

using namespace NP16;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Display display;
	display.Create(hInstance, hPrevInstance, lpCmdLine,
		nCmdShow, 1024, 768, 100, 100, L"Test Vulkan", L"VulkanClass");

	VulkanLoader vulkanLoader;
	vulkanLoader.Win32_LoadVulkan();
	vulkanLoader.CreateInstance();
	vulkanLoader.ReportExtensions();

	MSG msg;
	bool done = false;

	while (!done)
	{
		PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display.ReDraw();
	}

	return msg.wParam;
}