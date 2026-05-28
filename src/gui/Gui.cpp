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
    ImGui_ImplOpenGL3_Init("#version 130");

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

        // shop start

        float slotWidth = 180.0f;
        float slotHeight = 120.0f;
        float gap = 10.0f;
        float bottomPadding = 40.0f;

        float rerollWidth = 120.0f;
        float rerollHeight = 120.0f;
        float rerollGap = 20.0f;

        float totalShopWidth = rerollWidth + rerollGap + 5.0f * slotWidth + 4.0f * gap;

        ImVec2 windowSize = ImGui::GetWindowSize();

        float shopX = (windowSize.x - totalShopWidth) / 2.0f;
        float shopY = windowSize.y - slotHeight - bottomPadding;

        ImGui::SetCursorPos(ImVec2(shopX, shopY));

        ImGui::SetCursorPos(ImVec2(shopX, shopY));

        if (ImGui::Button("Reroll", ImVec2(rerollWidth, rerollHeight))) {
            engine.roll();
        }

        ImGui::SameLine(0.0f, rerollGap);

        for (int i = 0; i < engine.gamestate.shop.size(); i++) {
            Champion& champ = engine.gamestate.shop[i];

            std::string label;

            if (champ.id == 0) {
                label = "Empty##shop" + std::to_string(i);
            } else {
                label =
                    champ.name +
                    "\nCost: " + std::to_string(champ.cost) +
                    "\nStar: " + std::to_string(champ.starLevel) +
                    "##shop" + std::to_string(i);
            }

            if (ImGui::Button(label.c_str(), ImVec2(slotWidth, slotHeight))) {
                engine.buy(i);
            }

            if (i < engine.gamestate.shop.size() - 1) {
                ImGui::SameLine(0.0f, gap);
            }
        }
        // shop end

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