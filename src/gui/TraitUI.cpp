#include "TraitUI.h"
#include "Globals.h"
#include "Engine.h"
#include "raylib.h"


TraitUI::TraitUI(/* args */)
{
}

TraitUI::~TraitUI()
{   
}

void TraitUI::init() {
    traitFont = LoadFontEx("assets/Fonts/Spiegel.otf", 32, 0, 0);
    uiFont = LoadFontEx("assets/Fonts/Beaufort.otf", 32, 0, 0);

    SetTextureFilter(traitFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR);
}

void TraitUI::shutdown() {
    if (traitFont.texture.id != 0) UnloadFont(traitFont);
    if (uiFont.texture.id != 0) UnloadFont(uiFont);

    traitFont = {};
    uiFont = {};
}

Rectangle TraitUI::TraitBarRect() {
    float H = (float)GetScreenHeight();

    float barH   = H * 0.54;
    float barW   = barH * 0.145;
    float bottom = 0;

    return { 0.0f, (H - barH) / 2.15f, barW, barH };
}

void TraitUI::DrawTraitHexs(Engine& engine) {
    const auto* traitTable = &Set18::ALL_TRAITS;
    const auto* nameTable = &Set18::TRAIT_ID_TO_NAME;

    if (engine.gamestate.activeSet == SetId::Set17) {
        // traitTable = &Set17::ALL_TRAITS;
        // nameTable = &Set17::TRAIT_ID_TO_NAME;
    }

    Rectangle bar = TraitBarRect();
    const float top = bar.y + bar.height * 0.085f;
    const float slotH = bar.height * 0.086f;
    const float cx = bar.width * 0.92f;
    const int total = (int)engine.gamestate.activeTraits.size();

    int index = 0;
    int drawn = 0;

    for (const auto& entry : engine.gamestate.activeTraits) {
        const int traitId    = entry.id;
        const int traitCount = entry.count;
        if (index < scrollOffset) {
            index++;
            continue;
        }

        if (drawn >= 9) break;

        const float cy = top + drawn * slotH + slotH / 2.0f;

        std::string thresholds;

        auto tiersIt = traitTable->find(traitId);
        if (tiersIt != traitTable->end()) {
            for (size_t i = 0; i < tiersIt->second.first.size(); i++) {
                if (i > 0) thresholds += " > ";
                thresholds += std::to_string(tiersIt->second.first[i]);
            }
        }

        auto nameIt = nameTable->find(traitId);
        const std::string name = nameIt != nameTable->end() ? nameIt->second : "";

        const float rowH = slotH * 0.75f;
        const float pad = rowH * 0.2f;
        const float hexRadius = bar.width * 0.23f;
        const float numberH = rowH * 0.62f;
        const float numberW = numberH * 0.9f;
        const float numberX = cx + hexRadius + rowH * 0.16f;
        const float textX = numberX + numberW + pad;

        float nameW = MeasureTextEx(traitFont, name.c_str(), 15.0f, 0.5f).x;
        float thresholdW = MeasureTextEx(traitFont, thresholds.c_str(), 13.0f, 0.5f).x;
        float textW = std::max(nameW, thresholdW);

        DrawRectangle((int)cx, (int)(cy - rowH / 2.0f), (int)((textX - cx) + textW + pad), (int)rowH, Fade(DARKGRAY, 0.6f));
        DrawRectangle((int)numberX, (int)(cy - numberH / 2.0f), (int)numberW, (int)numberH, Fade(WHITE, 0.5f));

        const char* numberText = TextFormat("%d", traitCount);
        Vector2 numberSize = MeasureTextEx(traitFont, numberText, 20.0f, 0.5f);

        DrawTextEx(traitFont, numberText, {numberX + numberW / 2.0f - numberSize.x / 2.0f, cy - numberSize.y / 2.0f}, 20.0f, 0.5f, WHITE);

        Vector2 nameSize = MeasureTextEx(traitFont, name.c_str(), 15.0f, 0.5f);
        Vector2 thresholdSize = MeasureTextEx(traitFont, thresholds.c_str(), 13.0f, 0.5f);

        float textGap = 1.0f;
        float totalTextHeight = nameSize.y + textGap + thresholdSize.y;

        float nameY = cy - totalTextHeight / 2.0f;
        float thresholdY = nameY + nameSize.y + textGap;

        DrawTextEx(traitFont, name.c_str(), {textX, nameY}, 15.0f, 0.5f, WHITE);
        DrawTextEx(traitFont, thresholds.c_str(), {textX, thresholdY}, 13.0f, 0.5f, GRAY);
        DrawPoly({cx, cy}, 6, hexRadius, 30.0f, SKYBLUE);

        drawn++;
        index++;
    }

    const int remaining = total - scrollOffset - drawn;
    const float size = bar.width * 0.21f;
    const float overflowY = top + 9.0f * slotH + slotH / 2.0f;

    if (remaining > 0) {
        Vector2 points[5] = {
            {cx - size, overflowY + size * 0.4f},
            {cx, overflowY + size},
            {cx + size, overflowY + size * 0.4f},
            {cx + size, overflowY - size},
            {cx - size, overflowY - size}
        };

        DrawTriangleFan(points, 5, SKYBLUE);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Rectangle overflowRect = {cx - size, overflowY - size, size * 2.0f, size * 2.0f};
        bool clickedOverflow = remaining > 0 && CheckCollisionPointRec(GetMousePosition(), overflowRect);

        scrollOffset = clickedOverflow
            ? (scrollOffset + 9 >= total ? 0 : scrollOffset + 9)
            : 0;
    }
}

void TraitUI::DrawTraits(Engine& engine) {
    Rectangle bar = TraitBarRect();

    DrawRectangleLinesEx(bar, 2.0f, SKYBLUE);
    DrawTraitHexs(engine);
}