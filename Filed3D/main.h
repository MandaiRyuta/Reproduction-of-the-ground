#pragma once

#include <Windows.h>
#include <time.h>
#include <crtdbg.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#ifndef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#endif
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XAudio2.lib")

namespace WINDOWS_RECT
{
	constexpr int SCREEN_WIDTH = 1366;
	constexpr int SCREEN_HEIGHT = 768;
}
