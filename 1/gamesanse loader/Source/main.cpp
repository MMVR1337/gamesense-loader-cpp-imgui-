#include "../includes.h"

int main() {
    //KeyAuthApp_Init();  //initialization key auth or other auth system
    ImGuiInitializate();
    while (true) {
        //SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
        renderImgui([&]() {
            delta = ImGui::GetIO().DeltaTime;
            if (pages[0]) {
                LoginPage();
            }
            else if (pages[1]) {
                //main
            }
            else if (pages[2]) {
                //settings
            }
            else if (pages[3]) {
                //configs
            }
            MoveWnd(screen);
            });
    }
    ImGuiCleanupScene();
    return 0;
}
