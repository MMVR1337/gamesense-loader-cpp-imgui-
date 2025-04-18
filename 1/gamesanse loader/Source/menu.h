
void LoginPage() {
	ImVec2 p = ImGui::GetWindowPos();

	scpiw(ImVec2(50, 30), 156);
	SkeetInputText(u8"##username", LBuff, sizeof(LBuff), u8"username"); ImGui::PopItemWidth();

	scpiw(ImVec2(50, 65), 156);
	SkeetInputText(u8"##password", PBuff, sizeof(PBuff), u8"password"); ImGui::PopItemWidth();

	scpiw(ImVec2(80, 100), 156);
	if (SkeetButton("Login", ImVec2(95, 20))) {
		
	}

	scpiw(ImVec2(80, 130), 156); 
	if (SkeetButton("Exit", ImVec2(95, 20))) {
		exit(1337);
	}

	ImGui::PopItemWidth();
}