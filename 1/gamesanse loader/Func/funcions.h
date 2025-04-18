#pragma once

std::wstring GenerateRandomTitle(int length) {
    const wchar_t charset[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const size_t max_index = (sizeof(charset) / sizeof(*charset)) - 1;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, max_index);

    std::wstring random_string;
    for (int i = 0; i < length; ++i) {
        random_string += charset[dist(generator)];
    }

    return random_string;
}

void scpiw(ImVec2 pos, int Width) {
    ImGui::SetCursorPos(pos); ImGui::PushItemWidth(Width);
}

bool SkeetButton(const char* label, const ImVec2& size = ImVec2(0, 0)) {
    ImVec4 originalButtonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImVec4 originalButtonHoveredColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
    ImVec4 originalButtonActiveColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

    ImVec4 darkColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, darkColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, darkColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, darkColor);

    bool result = ImGui::Button(label, size);

    ImGui::PopStyleColor(3);

    return result;
}

bool SkeetInputText(const char8_t* label, char* buf, size_t buf_size, const char8_t* placeholder) {
    ImVec4 originalInputTextColor = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
    ImVec4 originalTextColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);

    ImVec4 darkColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, darkColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);

    std::string label_str(reinterpret_cast<const char*>(label));
    std::string placeholder_str(reinterpret_cast<const char*>(placeholder));

    bool active = ImGui::InputText(label_str.c_str(), buf, buf_size);

    ImGui::PopStyleColor(2);

    if (buf[0] == '\0' && !ImGui::IsItemActive()) {
        ImGui::SetCursorPosX(ImGui::GetItemRectMin().x + ImGui::GetStyle().FramePadding.x);
        ImGui::SetCursorPosY(ImGui::GetItemRectMin().y + ImGui::GetStyle().FramePadding.y);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", placeholder);
    }

    return active;
}