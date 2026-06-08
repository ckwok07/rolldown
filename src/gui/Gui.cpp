#include "Gui.h"
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GUI::GUI() {}

GUI::~GUI() {
    shutdown();
}

bool GUI::init() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "Rolldown Simulator", nullptr, nullptr);

    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    engine.initGameState();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    return true;
}

void GUI::run() {
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // debugging
        ImGui::Begin("TFT Debug");

        ImGui::Text("Gold: %d", engine.gamestate.gold);
        ImGui::Text("XP: %d", engine.gamestate.xp);
        ImGui::Text("Level: %d", engine.gamestate.level);
        ImGui::Text("Board Units: %d", engine.gamestate.boardUnitCount);

        ImGui::Separator();
        if (ImGui::Button("Reroll")) {
            engine.roll();
        }

        ImGui::Separator();

        ImGui::Text("Shop:");

        for (int i = 0; i < engine.gamestate.shop.size(); i++) {
            Champion& champ = engine.gamestate.shop[i];

            if (champ.id == 0) {
                ImGui::Text("Shop %d: Empty", i);
            } else {
                ImGui::Text(
                    "Shop %d: %s | Cost: %d | Star: %d",
                    i,
                    champ.name.c_str(),
                    champ.cost,
                    champ.starLevel
                );
            }

            std::string buttonLabel = "Buy##shop" + std::to_string(i);

            if (ImGui::Button(buttonLabel.c_str())) {
                engine.buy(i);
            }
        }

        ImGui::Separator();

        ImGui::Text("Bench:");
        for (int i = 0; i < engine.gamestate.bench.size(); i++) {
            Champion& champ = engine.gamestate.bench[i];

            if (champ.id == 0) {
                ImGui::Text("Bench %d: Empty", i);
            } else {
                ImGui::Text(
                    "Bench %d: %s | Cost: %d | Star: %d",
                    i,
                    champ.name.c_str(),
                    champ.cost,
                    champ.starLevel
                );
            }
        }

        ImGui::Separator();

        ImGui::Text("Board:");
        for (int r = 0; r < engine.gamestate.board.size(); r++) {
            for (int c = 0; c < engine.gamestate.board[r].size(); c++) {
                Champion& champ = engine.gamestate.board[r][c];

                if (champ.id == 0) {
                    ImGui::Text("Board [%d][%d]: Empty", r, c);
                } else {
                    ImGui::Text(
                        "Board [%d][%d]: %s | Cost: %d | Star: %d",
                        r,
                        c,
                        champ.name.c_str(),
                        champ.cost,
                        champ.starLevel
                    );
                }
            }
        }
        ImGui::End();
        // debug end

        // main layout
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Root", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse
        );

        float shopBarWidth = 1230.0f;
        float shopBarHeight = 170.0f;
        float bottomPadding = 40.0f;

        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 windowPos = ImGui::GetWindowPos();

        float shopBarX = (windowSize.x - shopBarWidth) / 2.0f;
        float shopBarY = windowSize.y - shopBarHeight - bottomPadding;

        ImVec2 rectMin(
            windowPos.x + shopBarX,
            windowPos.y + shopBarY
        );

        ImVec2 rectMax(
            rectMin.x + shopBarWidth,
            rectMin.y + shopBarHeight
        );

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        // bottom gui
        drawList->AddRectFilled(
            rectMin,
            rectMax,
            IM_COL32(0, 0, 0, 255)
        );

        drawList->AddRect(
            rectMin,
            rectMax,
            ImGui::GetColorU32(ImGuiCol_Border)
        );

        // shop start

        float slotWidth = 180.0f;
        float slotHeight = 120.0f;
        float gap = 10.0f;

        float totalShopWidth = 5.0f * slotWidth + 4.0f * gap;

        float shopX = (windowSize.x - totalShopWidth) / 2.0f;
        float shopY = windowSize.y - slotHeight - bottomPadding;

        ImGui::SetCursorPos(ImVec2(shopX, shopY));

        ImGui::SetCursorPos(ImVec2(shopX, shopY));

        drawList = ImGui::GetWindowDrawList();

        int buyIndexToCommit = -1;

        if (ImGui::Button("Reroll", ImVec2(120.0f, slotHeight))) {
            engine.roll();
        }

        ImGui::SameLine(0.0f, gap);

        for (int i = 0; i < engine.gamestate.shop.size(); i++) {
            Champion champ = engine.gamestate.shop[i];

            std::string hitboxId = "ShopCardHitbox##" + std::to_string(i);

            bool clicked = ImGui::InvisibleButton(
                hitboxId.c_str(),
                ImVec2(slotWidth, slotHeight)
            );

            ImVec2 min = ImGui::GetItemRectMin();
            ImVec2 max = ImGui::GetItemRectMax();

            bool hovered = ImGui::IsItemHovered();

            if (!draggingShop &&
                champ.id != 0 &&
                ImGui::IsItemActive() &&
                ImGui::IsMouseDragging(ImGuiMouseButton_Left, 3.0f)) {

                draggingShop = true;
                draggedShopIndex = i;
                draggedShopChamp = champ;

                ImVec2 mouse = ImGui::GetMousePos();
                dragOffsetX = mouse.x - min.x;
                dragOffsetY = mouse.y - min.y;
            }

            if (clicked && !draggingShop && champ.id != 0) {
                buyIndexToCommit = i;
            }

            bool hideThisSlot = draggingShop && draggedShopIndex == i;

            ImU32 fillColor;
            if (hideThisSlot || champ.id == 0) {
                fillColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
            } else if (hovered) {
                fillColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
            } else {
                fillColor = ImGui::GetColorU32(ImGuiCol_Button);
            }

            ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
            ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

            drawList->AddRectFilled(min, max, fillColor);
            drawList->AddRect(min, max, borderColor);

            if (!hideThisSlot && champ.id != 0) {
                drawList->AddText(
                    ImVec2(min.x + 10.0f, min.y + 10.0f),
                    textColor,
                    champ.name.c_str()
                );

                std::string costText = "Cost: " + std::to_string(champ.cost);
                drawList->AddText(
                    ImVec2(min.x + 10.0f, min.y + 32.0f),
                    textColor,
                    costText.c_str()
                );

                std::string starText = "Star: " + std::to_string(champ.starLevel);
                drawList->AddText(
                    ImVec2(min.x + 10.0f, min.y + 54.0f),
                    textColor,
                    starText.c_str()
                );
            }

            if (i < engine.gamestate.shop.size() - 1) {
                ImGui::SameLine(0.0f, gap);
            }
        }

        if (draggingShop && draggedShopIndex != -1) {
            ImVec2 mouse = ImGui::GetMousePos();

            ImVec2 dragMin(
                mouse.x - dragOffsetX,
                mouse.y - dragOffsetY
            );

            ImVec2 dragMax(
                dragMin.x + slotWidth,
                dragMin.y + slotHeight
            );

            ImDrawList* foreground = ImGui::GetForegroundDrawList();

            foreground->AddRectFilled(
                dragMin,
                dragMax,
                ImGui::GetColorU32(ImGuiCol_ButtonActive),
                6.0f
            );

            foreground->AddRect(
                dragMin,
                dragMax,
                ImGui::GetColorU32(ImGuiCol_Border),
                6.0f
            );

            foreground->AddText(
                ImVec2(dragMin.x + 10.0f, dragMin.y + 10.0f),
                ImGui::GetColorU32(ImGuiCol_Text),
                draggedShopChamp.name.c_str()
            );

            std::string costText = "Cost: " + std::to_string(draggedShopChamp.cost);
            foreground->AddText(
                ImVec2(dragMin.x + 10.0f, dragMin.y + 32.0f),
                ImGui::GetColorU32(ImGuiCol_Text),
                costText.c_str()
            );

            std::string starText = "Star: " + std::to_string(draggedShopChamp.starLevel);
            foreground->AddText(
                ImVec2(dragMin.x + 10.0f, dragMin.y + 54.0f),
                ImGui::GetColorU32(ImGuiCol_Text),
                starText.c_str()
            );

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                buyIndexToCommit = draggedShopIndex;

                draggingShop = false;
                draggedShopIndex = -1;
                draggedShopChamp = nullChamp;
                dragOffsetX = 0.0f;
                dragOffsetY = 0.0f;
            }
        }

        if (buyIndexToCommit != -1) {
            engine.buy(buyIndexToCommit);
        }
        // shop end

        // bench start

        float benchSlotSize = 70.0f;
        float benchGap = 0.0f;
        float benchAboveShopGap = 60.0f;

        float totalBenchWidth = 9.0f * benchSlotSize + 8.0f * benchGap;

        float benchX = (windowSize.x - totalBenchWidth) / 2.0f;
        float benchY = shopY - benchSlotSize - benchAboveShopGap;

        ImGui::SetCursorPos(ImVec2(benchX, benchY));

        ImDrawList* benchDrawList = ImGui::GetWindowDrawList();

        int benchDropTarget = -1;

        for (int i = 0; i < engine.gamestate.bench.size(); i++) {
            Champion champ = engine.gamestate.bench[i];

            std::string hitboxId = "BenchSlotHitbox##" + std::to_string(i);

            ImGui::InvisibleButton(
                hitboxId.c_str(),
                ImVec2(benchSlotSize, benchSlotSize)
            );

            ImVec2 min = ImGui::GetItemRectMin();
            ImVec2 max = ImGui::GetItemRectMax();

            bool hovered = ImGui::IsMouseHoveringRect(min, max);

            if (hovered) {
                benchDropTarget = i;
            }

            if (!draggingBench &&
                champ.id != 0 &&
                ImGui::IsItemActive() &&
                ImGui::IsMouseDragging(ImGuiMouseButton_Left, 3.0f)) {

                draggingBench = true;
                draggedBenchIndex = i;
                draggedBenchChamp = champ;

                ImVec2 mouse = ImGui::GetMousePos();
                benchDragOffsetX = mouse.x - min.x;
                benchDragOffsetY = mouse.y - min.y;
            }

            bool hideThisSlot = draggingBench && draggedBenchIndex == i;

            ImU32 fillColor = hovered
                ? ImGui::GetColorU32(ImGuiCol_FrameBgHovered)
                : ImGui::GetColorU32(ImGuiCol_FrameBg);

            ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
            ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

            benchDrawList->AddRectFilled(min, max, fillColor);
            benchDrawList->AddRect(min, max, borderColor);

            if (!hideThisSlot && champ.id != 0) {
                benchDrawList->AddText(
                    ImVec2(min.x + 6.0f, min.y + 8.0f),
                    textColor,
                    champ.name.c_str()
                );

                std::string starText = std::to_string(champ.starLevel) + "*";

                benchDrawList->AddText(
                    ImVec2(min.x + 6.0f, min.y + 28.0f),
                    textColor,
                    starText.c_str()
                );
            }

            if (i < engine.gamestate.bench.size() - 1) {
                ImGui::SameLine(0.0f, benchGap);
            }
        }

        if (draggingBench && draggedBenchIndex != -1) {
            ImVec2 mouse = ImGui::GetMousePos();

            ImVec2 dragMin(
                mouse.x - benchDragOffsetX,
                mouse.y - benchDragOffsetY
            );

            ImVec2 dragMax(
                dragMin.x + benchSlotSize,
                dragMin.y + benchSlotSize
            );

            ImDrawList* foreground = ImGui::GetForegroundDrawList();

            foreground->AddRectFilled(
                dragMin,
                dragMax,
                ImGui::GetColorU32(ImGuiCol_FrameBgHovered)
            );

            foreground->AddRect(
                dragMin,
                dragMax,
                ImGui::GetColorU32(ImGuiCol_Border)
            );

            foreground->AddText(
                ImVec2(dragMin.x + 6.0f, dragMin.y + 8.0f),
                ImGui::GetColorU32(ImGuiCol_Text),
                draggedBenchChamp.name.c_str()
            );

            std::string starText = std::to_string(draggedBenchChamp.starLevel) + "*";

            foreground->AddText(
                ImVec2(dragMin.x + 6.0f, dragMin.y + 28.0f),
                ImGui::GetColorU32(ImGuiCol_Text),
                starText.c_str()
            );

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                if (benchDropTarget != -1 && benchDropTarget != draggedBenchIndex) {
                    engine.benchtobench(draggedBenchIndex, benchDropTarget);
                }

                draggingBench = false;
                draggedBenchIndex = -1;
                draggedBenchChamp = nullChamp;
                benchDragOffsetX = 0.0f;
                benchDragOffsetY = 0.0f;
            }
        }

        // bench end

        ImGui::End();
        // main layout end

        ImGui::Render();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GUI::shutdown() {
    if (window != nullptr) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}