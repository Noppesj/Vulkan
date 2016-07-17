#pragma once

#include <Windows.h>
#include <string>

class Display
{
private:
	int m_Width = 800;
	int m_Height = 600;
	int m_PosX = 100;
	int m_PosY = 100;

	std::wstring m_Title = L"Defualt Title";
	std::wstring m_WindowClassName = L"Default WindowName";

	HWND m_windowHandler;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
public:
	Display();
	~Display();

	void Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
		int nCmdShow, int width, int height,
		int posX, int posY, const std::wstring& title,
		const std::wstring& windowClassName);

	void ReDraw();
};