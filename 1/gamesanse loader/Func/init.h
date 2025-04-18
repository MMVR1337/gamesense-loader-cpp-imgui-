#pragma once

void MoveWnd(ImVec2 MAX) {
    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::InvisibleButton("Move_detector", MAX));
    if (ImGui::IsItemActive()) {
        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, screen.x, screen.y, TRUE);
    }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

inline void StopRender() {
    const float clear_color_with_alpha[4] = { 0 };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0);
}

inline void StartRender() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            done = true;
    }
    if (done != false) {
        exit(0);
    }
    if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
    {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        g_ResizeWidth = g_ResizeHeight = 0;
        CreateRenderTarget();
    }
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

inline void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE: {
        if (wParam == SIZE_MINIMIZED)
            return 0;
        UINT newWidth = LOWORD(lParam);
        UINT newHeight = HIWORD(lParam);

        if (newWidth != g_ResizeWidth || newHeight != g_ResizeHeight) {
            g_ResizeWidth = newWidth;
            g_ResizeHeight = newHeight;
        }

        return 0;
    }
    case WM_SYSCOMMAND: {
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
    }
                      break;
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void ImGuiCleanupScene() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

template <typename Func> void renderImgui(Func&& code) {

    StartRender();
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(screen);
        ImGui::Begin("@", &done, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
            code();
        }
        ImGui::End();
    }
    ImGui::Render();
    StopRender();
}

void Init_WindowClass() {

    ImGui_ImplWin32_EnableDpiAwareness();
    wc = {
        sizeof(wc),
        CS_VREDRAW | CS_HREDRAW,
        WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
        L"Classname", nullptr
    };
    wc.hbrBackground = NULL;
    ::RegisterClassExW(&wc);
}

void InitFonts() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig cfg;
    static struct rFonts {
        ImFont** fontname;
        unsigned char* bytes;
        int bytesize;
        int pixelsize;
        float size;
    };
    static auto InitFont = [&](ImFont** fontname, unsigned char* bytes, int bytesize, float size) -> void {
        *fontname = io.Fonts->AddFontFromMemoryTTF(bytes, bytesize, size, &cfg, io.Fonts->GetGlyphRangesCyrillic());
        };

    std::jthread a1([&] {
        static const unsigned regularsize = sizeof(Railway);
        for (unsigned int i = 3; i < NumOfSizes; i++) {
            InitFont(&railway[i], Railway, regularsize, sizes[i]);
        }
        });

    a1.join();
}

void Init_Hwnd() {

    hwnd = ::CreateWindowExW(
        NULL,                                         // Дополнительные стили
        wc.lpszClassName,                             // Имя зарегистрированного класса
        GenerateRandomTitle(1 + rand() % 8).c_str(),  // Заголовок окна
        WS_POPUP,                                     // Стили окна
        window.x,                                     // Позиция X
        window.y,                                     // Позиция Y
        screen.x,                                     // Ширина
        screen.y,                                     // Высота
        NULL,                                         // Родительское окно
        NULL,                                         // Меню
        GetModuleHandle(NULL),                        // Дескриптор приложения
        NULL                                          // Дополнительные параметры
    );

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        done = true;
    }

    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);


}

void Init_DirectX() {

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
    ImGui::CreateContext();
    ImGui::StyleColorsDark();


    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);


    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

}

void ImGuiInitializate() {

    Init_WindowClass();

    Init_Hwnd();

    Init_DirectX();

    std::jthread init_font(InitFonts);

    init_font.join();
}