#pragma once

MARGINS margins;
RECT rc;
HWND hwnd;
WNDCLASSEXW wc;

float g_DpiScale = 1.0f;

bool pages[4]{
	true,  // login
	false, // select
	false, // settings
	false  // configs
};


static const unsigned int NumOfSizes = 22;
float sizes[NumOfSizes] = {
    11.f, //0
    12.f, //1
    13.f, //2
    14.f, //3
    15.f, //4
    16.f, //5
    17.f, //6
    18.f, //7
    19.f, //8
    20.f, //9
    21.f, //10
    22.f, //11
    23.f, //12
    24.f, //13
    25.f, //14
    26.f, //15
    27.f, //16
    28.f, //17
    29.f, //18
    30.f, //19
    31.f, //20
    32.f  //21
};
ImFont* railway[NumOfSizes];

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
D3DX11_IMAGE_LOAD_INFO Iinfo;
ID3DX11ThreadPump* pump{ nullptr };