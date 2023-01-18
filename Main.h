std::string DecryptText(std::string txt, DWORD hex) {
    std::string rStr = "";
    for (char rC : txt) {
        if (rC != ' ') {
            rC -= hex;
            rStr += rC;
        }
        else
            rStr += ' ';
    }

    return rStr;
}

void setWindowSize() {
    float X = getGuiVec("Width").Value;
    float Y = getGuiVec("Height").Value;

    ui::window_size = { X, Y };
}

void setRecoil() {
    bool isRecoil = false, isRapidFire = false, isSecondary = false, isPrimary = true, canAutoRun = false;
    while (1) {
        if (bRecChk) {
            bRecChk = false;
            break;
        }
        if (GetAsyncKeyState(VK_LBUTTON) && isRecoil && isPrimary) {
            mouse_event(MOUSEEVENTF_MOVE, xVal, yVal, 0, 0);
            Sleep(20);
        }
        if (GetAsyncKeyState(VK_LBUTTON) && isRecoil && isSecondary) {
            mouse_event(MOUSEEVENTF_MOVE, xVal2, yVal2, 0, 0);
            Sleep(20);
        }
        if (GetAsyncKeyState(VK_LBUTTON) && isRapidFire) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            Sleep(rFreSpd);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(rFreSpd);
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, MOUSEEVENTF_ABSOLUTE);
            Sleep(rFreSpd);
        }
        if (GetAsyncKeyState(swapKey)) {
            if (isPrimary) {
                isPrimary = false;
                isSecondary = true;
            }
            else if (isSecondary) {
                isSecondary = false;
                isPrimary = true;
            }
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(togRapKey)) {
            if (isRapidFire) {
                isRapidFire = false;
            }
            else
                isRapidFire = true;
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(togAutoRunKey)) {
            if (canAutoRun) {
                canAutoRun = false;
            }
            else
                canAutoRun = true;
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(yIncrKey)) {
            yVal += 1;
            if (yVal > 30) {
                yVal = 30;
            }
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(yDecKey)) {
            yVal -= 1;
            if (yVal < -30) {
                yVal = -30;
            }
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(xIncrKey)) {
            xVal += 1;
            if (xVal > 30) {
                xVal = 30;
            }
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(xDecKey)) {
            xVal -= 1;
            if (xVal < -30) {
                xVal = -30;
            }
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(togRecKey)) {
            if (isRecoil) {
                isRecoil = false;
            }
            else
                isRecoil = true;
            Sleep(pressDelay);
        }
        if (GetAsyncKeyState(VK_F12)) {
            break;
        }
    }
}

void initKeys() {
    KeyVec swapVec = getKey("Swap");
    swapKey = swapVec.Key; swapKeyName = swapVec.Name;
    KeyVec yIncrVec = getKey("IncreaseY");
    yIncrKey = yIncrVec.Key; yIncr = yIncrVec.Name;
    KeyVec recoil = getKey("ToggleRecoil");
    togRecKey = recoil.Key; togRec = recoil.Name;
    KeyVec rapid = getKey("ToggleRapidFire");
    togRap = rapid.Name; togRapKey = rapid.Key;
    KeyVec incrX = getKey("IncreaseX");
    xIncrKey = incrX.Key; xIncr = incrX.Name;
    KeyVec decX = getKey("DecreaseX");
    xDec = decX.Name; xDecKey = decX.Key;
    KeyVec decY = getKey("DecreaseY");
    yDec = decY.Name; yDecKey = decY.Key;
}

void ui::render() {
    if (!globals.active) return;
    bool tmpBChk = false; int i = 0, countTillNext = 0;
    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);
    std::vector<Vec2d> vecList = getData();
    initKeys();
    setWindowSize();
    pressDelay = getGuiVec("KeyDelay").Value;
    countTillNext = getGuiVec("CountTillNextLine").Value;
    ImGui::Begin(window_title, &globals.active, window_flags);
    { 
        if (ImGui::Checkbox(DecryptText("Jsfgqj",0x5).c_str(), &bRec)) {
            if (bRec) {
                std::thread thr(setRecoil);
                thr.detach();
            }
            else {
                bRecChk = true;
            }
        }
        ImGui::SliderInt(DecryptText("^ ", 0x5).append("Primary").c_str(), &yVal, -30, 30);
        ImGui::SliderInt(DecryptText("] ", 0x5).append("Primary").c_str(), &xVal, -30, 30);
        ImGui::SliderInt("Y Secondary", &yVal2, -30, 30);
        ImGui::SliderInt("X Secondary", &xVal2, -30, 30);
        ImGui::SliderInt(DecryptText("Wfuni Knwj Ijqf~-rx.",0x5).c_str(), &rFreSpd, 0, 1000);
        ImGui::Text(togRec.append(": Toggle Recoil  " + togRap + ": Toggle Rapid Fire  ")
            .append(swapKeyName).append(": Swap Values").c_str());
        ImGui::Text(yIncr.append(": Increase Y   ").append(yDec + ": Decrease Y").c_str());
        ImGui::Text(xIncr.append(": Increase X   ").append(xDec + ": Decrease X").c_str());
        ImGui::NewLine();
        for (Vec2d vec : vecList) {
            if (i == countTillNext) {
                i = 0;
                if (ImGui::Checkbox(vec.Name.c_str(), &tmpBChk)) {
                    yVal = vec.y; xVal = vec.x;
                    yVal2 = vec.y2, xVal2 = vec.x2;
                }
            }
            else {
                ImGui::SameLine();
                if(ImGui::Checkbox(vec.Name.c_str(), &tmpBChk)) {
                    yVal = vec.y; xVal = vec.x;
                    yVal2 = vec.y2, xVal2 = vec.x2;
                }
            }
            i++;
        }
        
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;
	
    // colors
    GuiVecStr guiColor = getGuiVecStr("Color");
    if (guiColor.Value == "Classic") {
        ImGui::StyleColorsClassic();
    }
    else if (guiColor.Value == "Dark") {
        ImGui::StyleColorsDark();
    }
    else if (guiColor.Value == "Light") {
        ImGui::StyleColorsLight();
    }
    else {
        ImGui::StyleColorsDark();
    }

	if (window_pos.x == 0) {
		RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
		window_pos = (screen_res - window_size) * 0.5f;

		// init images here
	}
}
