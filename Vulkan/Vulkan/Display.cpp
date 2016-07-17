#include "Display.h"

Display::Display()
{
}

Display::~Display()
{
}

LRESULT CALLBACK Display::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	default:
	{
		break;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Display::Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nCmdShow, int width, int height,
	int posX, int posY, const std::wstring& title,
	const std::wstring& windowClassName)
{
	WNDCLASSEX windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = (LPCWSTR)windowClassName.c_str();
	RegisterClassEx(&windowClass);

	LPCWSTR test = (LPCWSTR)title.c_str();
	m_windowHandler = CreateWindowEx(NULL,
		windowClassName.c_str(),
		title.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		posX,
		posY,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	m_Width = width;
	m_Height = height;
	m_PosX = posX;
	m_PosY = posY;

	m_Title = title;
	m_WindowClassName = windowClassName;
}

void Display::ReDraw()
{
	RedrawWindow(m_windowHandler, NULL, NULL, RDW_INTERNALPAINT);
}