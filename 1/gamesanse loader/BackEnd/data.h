#pragma once

static bool done = false;
float delta;

ImVec2 screen = { 256, 180 };
ImVec2 navpos = { (screen.x / 2) - (216 / 2), screen.y + 250 };
ImVec2 window = { (GetSystemMetrics(SM_CXSCREEN) / 2) - (screen.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (screen.y / 2) };

static char LBuff[128];
static char PBuff[128];

std::wstring OpenFileDialog() {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L" ";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameW(&ofn)) { return std::wstring(fileName); }
    return L"";
}