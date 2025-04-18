#include <Windows.h>
#include <dwmapi.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <cstring>
#include <ShlObj.h>
#include <TlHelp32.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <urlmon.h>
#include <stdio.h>
#include <algorithm> 
#include <stdlib.h>
#include <time.h>
#include <random>
#include <mutex>
#include <random>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#include <d3d11.h>
#include <D3DX11.h>

#include "Func/funcions.h"

#include "Source/bytes.h"
#include "BackEnd/data.h"
#include "BackEnd/backend.h"
#include "Func/init.h"

#include "Source/menu.h"

#pragma comment(lib, "WindowsApp.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dx11.lib")