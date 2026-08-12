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

        Rectangle rowRect = {(float)cx, cy - rowH / 2.0f, (textX - cx) + textW + pad, rowH};
        DrawRectangleRounded(rowRect, 0.18f, 8, Fade(DARKGRAY, 0.6f));

        Rectangle numberRect = {numberX, cy - numberH / 2.0f, numberW, numberH};
        DrawRectangleRounded(numberRect, 0.18f, 8, Fade(WHITE, 0.5f));

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

void TraitUI::DrawTraits(Engine& engine, const Drag& drag) {
    Rectangle bar = TraitBarRect();

    // DrawRectangleLinesEx(bar, 2.0f, SKYBLUE);
    DrawTraitsVisuals();
    DrawTraitHexs(engine);
    DrawItems(engine.gamestate.items, drag);
    DrawDraggedItem(engine.gamestate.items, drag);
}

void TraitUI::DrawTraitsVisuals() {
    Rectangle rect = TraitBarRect();

    Rectangle bar = {rect.x, rect.y + rect.height * 0.05f, rect.width * 0.99f, rect.height * 0.90f};
    Rectangle upperrect = {rect.x, rect.y, rect.width * 0.60f, rect.height * 0.05f};
    Rectangle lowerrect = {rect.x, rect.y + bar.height + rect.height * 0.05f, rect.width * 0.60f, rect.height * 0.05f};

    auto DrawLayer = [&](float inset, Color color) {
        float slope = (bar.y - upperrect.y) / ((bar.x + bar.width) - (upperrect.x + upperrect.width));
        float offset = inset * sqrtf(1.0f + slope * slope);

        float topY = upperrect.y + inset;
        float rightX = bar.x + bar.width - inset;
        float lineB = upperrect.y - slope * (upperrect.x + upperrect.width) + offset;

        float joinX = (topY - lineB) / slope;
        float joinY = slope * rightX + lineB;

        float bottomY = lowerrect.y + lowerrect.height - inset;
        float bottomJoinY = rect.y + rect.height - (joinY - rect.y);

        Rectangle innerBar = {bar.x, joinY, rightX - bar.x, bottomJoinY - joinY};
        Rectangle innerUpperrect = {upperrect.x, topY, joinX - upperrect.x, joinY - topY};
        Rectangle innerLowerrect = {lowerrect.x, bottomJoinY, joinX - lowerrect.x, bottomY - bottomJoinY};

        DrawRectangleRec(innerBar, color);
        DrawRectangleRec(innerUpperrect, color);
        DrawRectangleRec(innerLowerrect, color);

        DrawTriangle({joinX, topY}, {joinX, joinY}, {rightX, joinY}, color);
        DrawTriangle({joinX, bottomJoinY}, {joinX, bottomY}, {rightX, bottomJoinY}, color);
    };

    DrawLayer(0.0f, Fade(BLACK, 0.75f));
    DrawLayer(1.0f, {111, 93, 57, 255});
    DrawLayer(4.5f, Fade(BLACK, 0.75f));
    DrawLayer(6.0f, {20, 31, 32, 255});
    DrawLayer(9.5f, Fade({42, 74, 72, 255}, 0.65f));
    DrawLayer(11.5f, {20, 31, 32, 255});

    float gap = bar.height * 0.030f;
    float squareSize = bar.width * 0.42f;
    float y = bar.y + bar.height * 0.028f;

    for (int i = 0; i < 10; i++) {
        Rectangle square = {bar.x + bar.width * 0.13f, y, squareSize, squareSize};
        DrawRectangleRec(square, BLACK);
        y = y + squareSize + gap;
    }
}

void TraitUI::DrawItems(const vector<Item>& items, const Drag& drag) {
    Rectangle rect = TraitBarRect();

    Rectangle bar = {rect.x, rect.y + rect.height * 0.05f, rect.width * 0.99f, rect.height * 0.90f};
    Rectangle upperrect = {rect.x, rect.y, rect.width * 0.60f, rect.height * 0.05f};
    Rectangle lowerrect = {rect.x, rect.y + bar.height + rect.height * 0.05f, rect.width * 0.60f, rect.height * 0.05f};

    float gap = bar.height * 0.030f;
    float squareSize = bar.width * 0.42f;
    float y = bar.y + bar.height * 0.028f;

    for (int i = 0; i < 10; i++) {
        Rectangle square = {bar.x + bar.width * 0.13f, y, squareSize, squareSize};

        const DragState& state = drag.GetState();
        if (i < items.size() && !(state.phase == DragPhase::Dragging && state.payload == DragPayload::Item && state.source.zone == Zone::Inventory && state.source.index == i)) {
            Texture2D* texture = GetItemTexture(items[i]);

            if (texture != nullptr) {
                DrawRectangleRec(square, {111, 93, 57, 255});
                Rectangle itemRect = {square.x + square.width * 0.025f, square.y + square.height * 0.025f, square.width * 0.95f, square.height * 0.95f};
                Rectangle source = {0, 0, (float)texture->width, (float)texture->height};
                DrawTexturePro(*texture, source, itemRect, {0, 0}, 0.0f, WHITE);
            }
        }
        y = y + squareSize + gap;
    }
}

string TraitUI::GetItemName(const Item& item) {
    if (holds_alternative<int>(item)) {
        int id = get<int>(item);
        auto it = Set18::itemComponents.find(id);
        return it != Set18::itemComponents.end() ? it->second : "";
    }

    pair<int, int> ids = get<pair<int, int>>(item);
    auto it = Set18::completedItems.find(ids);
    return it != Set18::completedItems.end() ? it->second : "";
}

Texture2D* TraitUI::GetItemTexture(const Item& item) {
    string itemName = GetItemName(item);
    if (itemName.empty()) return nullptr;

    auto existing = itemTextures.find(itemName);

    if (existing != itemTextures.end()) {
        return existing->second.id != 0 ? &existing->second : nullptr;
    }

    string assetName = itemName;

    for (char& ch : assetName) {
        if (ch == ' ') ch = '_';
    }

    string path = "assets/Set18/items/120px-" + assetName + "_TFT_item.png";

    if (!FileExists(path.c_str())) {
        TraceLog(LOG_WARNING, "Missing item texture: %s", path.c_str());
        itemTextures[itemName] = Texture2D{};
        return nullptr;
    }

    Texture2D texture = LoadTexture(path.c_str());
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

    itemTextures[itemName] = texture;
    return &itemTextures[itemName];
}

int TraitUI::GetHoveredItemSlot(Vector2 mousePos) {
    Rectangle rect = TraitBarRect();

    Rectangle bar = {rect.x, rect.y + rect.height * 0.05f, rect.width * 0.99f, rect.height * 0.90f};

    float gap = bar.height * 0.030f;
    float squareSize = bar.width * 0.42f;
    float y = bar.y + bar.height * 0.028f;

    for (int i = 0; i < 10; i++) {
        Rectangle square = {bar.x + bar.width * 0.13f, y, squareSize, squareSize};

        if (CheckCollisionPointRec(mousePos, square)) {
            return i;
        }

        y = y + squareSize + gap;
    }

    return -1;
}

void TraitUI::DrawDraggedItem(const vector<Item>& items, const Drag& drag) {
    const DragState& state = drag.GetState();

    if (state.phase != DragPhase::Dragging || state.payload != DragPayload::Item) return;
    if (state.source.index < 0 || state.source.index >= items.size()) return;

    Texture2D* texture = GetItemTexture(items[state.source.index]);
    if (texture == nullptr) return;

    Rectangle rect = TraitBarRect();
    Rectangle bar = {rect.x, rect.y + rect.height * 0.05f, rect.width * 0.99f, rect.height * 0.90f};

    float squareSize = bar.width * 0.42f;
    Vector2 mouse = GetMousePosition();

    Rectangle square = {mouse.x - squareSize / 2.0f, mouse.y - squareSize / 2.0f, squareSize, squareSize};

    DrawRectangleRec(square, {111, 93, 57, 255});

    Rectangle itemRect = {square.x + square.width * 0.025f, square.y + square.height * 0.025f, square.width * 0.95f, square.height * 0.95f};
    Rectangle source = {0, 0, (float)texture->width, (float)texture->height};

    DrawTexturePro(*texture, source, itemRect, {0, 0}, 0.0f, WHITE);
}