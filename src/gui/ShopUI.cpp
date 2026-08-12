#include "ShopUI.h"
#include "Drag.h"
#include "raymath.h"
#include "rlgl.h"

ShopUI::ShopUI(/* args */)
{
}

ShopUI::~ShopUI()
{
}

void ShopUI::init() {
    traitFont = LoadFontEx("assets/Fonts/Spiegel.otf", 32, 0, 0);
    uiFont = LoadFontEx("assets/Fonts/Beaufort.otf", 32, 0, 0);
    SetTextureFilter(traitFont.texture, TEXTURE_FILTER_BILINEAR);
}

void ShopUI::shutdown() {
    UnloadFont(traitFont);
    UnloadFont(uiFont);
    for (auto& [name, texture] : splashTextures) {
        if (texture.id != 0) UnloadTexture(texture);
    }
    traitFont = {};
    uiFont = {};
}

static Color CostTierColor(int cost) {
    switch (cost) {
        case 1: return Color{ 145, 145, 145, 255 }; // grey
        case 2: return Color{  50, 205, 100, 255 }; // green
        case 3: return Color{  65, 135, 255, 255 }; // blue
        case 4: return Color{ 175,  75, 235, 255 }; // purple
        case 5: return Color{ 245, 185,  45, 255 }; // gold
        default: return DARKGRAY;
    }
}


static pair<Color,Color> ShopTierColor(int cost) {
    switch (cost) {
        case 1: return {Color{29, 37, 38, 255}, Color{ 35,48,58, 255 }}; // grey
        case 2: return {Color{51, 68, 64, 255}, Color{18, 92, 51, 255 }}; // green
        case 3: return {Color{38, 56, 67, 255}, Color{27, 85, 124, 255}}; // blue
        case 4: return {Color{78, 27, 57, 255}, Color{ 160, 20, 138, 255}}; // purple
        case 5: return {Color{121, 97,50, 255}, Color{ 191,126, 20, 255 }}; // gold
        default: return {BLACK, DARKGRAY};
    }
}

Texture2D* ShopUI::GetChampionSplash(const std::string& championName) {
    if (championName.empty()) {
        return nullptr;
    }

    auto existing = splashTextures.find(championName);

    if (existing != splashTextures.end()) {
        return existing->second.id != 0
            ? &existing->second
            : nullptr;
    }

    std::string assetName;

    for (char ch : championName) {
        if (ch != ' ' && ch != '\'') {
            assetName += ch;
        }
    }

    std::string prefix = "Set18";
    std::string suffix = "TFT18_"; 
    // fix ts

    // if (engine.gamestate.activeSet == SetId::Set17) {
    //     prefix = "Set17";
    //     suffix = "TFT17_";
    // } else {
    //     prefix = "Set18";
    //     suffix = "TFT18_";
    // }
    //

    std::string path = "assets/" + prefix + "/" + suffix + assetName + ".png";

    if (!FileExists(path.c_str())) {
        TraceLog(
            LOG_WARNING,
            "Missing champion splash: %s",
            path.c_str()
        );

        splashTextures[championName] = Texture2D{};
        return nullptr;
    }

    Texture2D texture = LoadTexture(path.c_str());

    SetTextureFilter(
        texture,
        TEXTURE_FILTER_BILINEAR
    );

    splashTextures[championName] = texture;

    return &splashTextures[championName];
}

void ShopUI::DrawTextureCover( Texture2D texture, Rectangle destination, Color tint) {
    float textureAspect =
        (float)texture.width / (float)texture.height;

    float destinationAspect =
        destination.width / destination.height;

    Rectangle source = {
        0.0f,
        0.0f,
        (float)texture.width,
        (float)texture.height
    };

    if (textureAspect > destinationAspect) {
        source.width =
            (float)texture.height * destinationAspect;

        source.x =
            ((float)texture.width - source.width) / 2.0f;
    } else {
        source.height =
            (float)texture.width / destinationAspect;

        source.y =
            ((float)texture.height - source.height) / 2.0f;
    }

    DrawTexturePro( texture, source, destination, { 0.0f, 0.0f }, 0.0f, tint);
}

void ShopUI::DrawShopIcon(Rectangle rect, Champion& champion, bool highlighted, bool sourceHidden, bool samechamps) {
    // card boder
    float cardBorder = 10.0f;
    DrawRectangleLinesEx(rect, cardBorder, BLACK);

    float border = 2.5f;
    float infoHeight = rect.height * 0.18f;

    Rectangle innerRect = { rect.x + border, rect.y + border, rect.width - border * 2.0f, rect.height - border * 2.0f};

    // spash art rectangle
    float splashBorder = 4.0f;
    Rectangle artRect = {innerRect.x, innerRect.y, innerRect.width, innerRect.height - infoHeight};

    // splash out black ouline
    Rectangle splashRect = {artRect.x - splashBorder,artRect.y - splashBorder,artRect.width + splashBorder * 2.0f,artRect.height + splashBorder * 2.0f};

    // name rectangle
    Rectangle infoRect = {innerRect.x, artRect.y + artRect.height, innerRect.width, infoHeight};

    if (champion.id != 0 && !sourceHidden) {
        Texture2D* splash = GetChampionSplash(champion.name);

        DrawRectangleRec(splashRect, BLACK);
        DrawRectangleGradientEx(rect, ShopTierColor(champion.cost).first, ShopTierColor(champion.cost).first, ShopTierColor(champion.cost).second, ShopTierColor(champion.cost).second);

        if (splash != nullptr) {
            DrawTextureCover(*splash, artRect, WHITE);
        }

        int traitFontSize = 15;
        float traitSpacing = 24.0f;

        float hexRadius = 8.0f;
        float gap = 4.0f;

        float traitY = artRect.y + artRect.height - champion.traits.size() * traitSpacing;

        for (int j = 0; j < champion.traits.size(); j++) {
            float y = traitY + j * traitSpacing;

            Vector2 hexCenter = {artRect.x + gap + hexRadius, y + traitFontSize / 2.0f};
            DrawPoly(hexCenter, 6, hexRadius, 30.0f, GRAY);
            DrawPolyLinesEx(hexCenter, 6, hexRadius, 30.0f, 1.5f, BLACK);

            float traitX = hexCenter.x + hexRadius + gap;
            string traitName = Set18::TRAIT_ID_TO_NAME.at(champion.traits[j]);
            DrawTextEx(traitFont, traitName.c_str(), {traitX, y * 1.00f}, traitFontSize, 0.5f, WHITE);
        }

        DrawRectangleGradientEx(infoRect, ShopTierColor(champion.cost).first, ShopTierColor(champion.cost).first, ShopTierColor(champion.cost).second, ShopTierColor(champion.cost).second);

        int fontSize = (int)(infoRect.height * 0.60f);
        int textY = (int)(infoRect.y + (infoRect.height - fontSize)/ 1.5);

        // name
        DrawTextEx(uiFont, champion.name.c_str(), {(float)(infoRect.x + 6.0f), (float)textY}, fontSize, 1.0f, WHITE);

        // cost
        const char* costText = TextFormat("%d", champion.cost);

        Vector2 costSize = MeasureTextEx(traitFont, costText, 18, 1.0f);

        DrawTextEx(traitFont, costText, {infoRect.x + infoRect.width - costSize.x - 6.0f, infoRect.y + (infoRect.height - costSize.y) / 2.0f + 1.0f}, 18, 1.0f, WHITE);
    }

    if (samechamps) {
        DrawRectangleRec(rect, Fade(WHITE, 0.25f));
    }

    if (champion.id != 0 && !sourceHidden) {
        if (highlighted) {
            DrawRectangleRec(rect, Fade(WHITE, 0.25f));
        }
        DrawLineEx({infoRect.x, infoRect.y + 2.0f}, {infoRect.x + infoRect.width, infoRect.y}, 2.0f, Fade(WHITE, 0.07f));
        DrawRectangleLinesEx(rect, 2.0f, Fade(WHITE, 0.07f));
    }
}

Rectangle ShopUI::ShopBarRect() {
    float H = (float)GetScreenHeight();
    float W = (float)GetScreenWidth();

    float barH   = H * 0.1565f;
    float barW   = barH * 6.8f;
    float bottom = 0;

    return { (W - barW) / 2.0f + W * -0.042f, H - bottom - barH, barW, barH };
}

void ShopUI::DrawShopTrapezoid() {
    Rectangle shop = ShopBarRect();

    float centerX = (shop.x + shop.width / 2.0f) * 1.0925f;
    float height = shop.height * 0.25f;
    float bottomWidth = shop.width * 0.14f;
    float topWidth = bottomWidth * 0.60f;
    float thickness = 3.0f;

    Vector2 topLeft = {centerX - topWidth / 2.0f, shop.y - height};
    Vector2 topRight = {centerX + topWidth / 2.0f, shop.y - height};
    Vector2 bottomRight = {centerX + bottomWidth / 2.0f, shop.y};
    Vector2 bottomLeft = {centerX - bottomWidth / 2.0f, shop.y};

    DrawLineEx(topLeft, topRight, thickness, SKYBLUE);
    DrawLineEx(topRight, bottomRight, thickness, SKYBLUE);
    DrawLineEx(bottomRight, bottomLeft, thickness, SKYBLUE);
    DrawLineEx(bottomLeft, topLeft, thickness, SKYBLUE);
}

Rectangle ShopUI::LockRect() {
    Rectangle shop = ShopBarRect();
    Rectangle lockRect = {shop.x + shop.width - shop.width * 0.062f, shop.y - shop.height * 0.21f, shop.width * 0.062f, shop.height * 0.21f};

    return lockRect;
}

Rectangle ShopUI::InnerShopRect() {
    Rectangle bar = ShopBarRect();
    return {bar.x + bar.height * 0.04f, bar.y + bar.height * 0.04f, bar.width - bar.height * 0.08f, bar.height - bar.height * 0.04f};
}

Rectangle ShopUI::InnerLevelRect() {
    Rectangle bar = ShopBarRect();
    Rectangle levelRect = {bar.x, bar.y - bar.height * 0.25f, bar.width * 0.15f, bar.height * 0.25f};
    float triangleWidth = bar.width * 0.028f;

    float levelInset = levelRect.height * 0.12f;
    float rightInset = levelInset * (sqrtf(levelRect.height * levelRect.height + triangleWidth * triangleWidth) - triangleWidth) / levelRect.height;

    return {levelRect.x + levelInset, levelRect.y + levelInset, levelRect.width - levelInset - rightInset, levelRect.height - levelInset * 2.0f};
}

Rectangle ShopUI::ShopXpRect() {
    Rectangle bar = ShopBarRect();
    return {
        bar.x + bar.width  * 0.009f,
        bar.y + bar.height * 0.08f,
        bar.width  * 0.155f,
        bar.height * 0.39f
    };
}

Rectangle ShopUI::ShopRerollRect() {
    Rectangle bar = ShopBarRect();
    return {
        bar.x + bar.width  * 0.009f,
        bar.y + bar.height * 0.52f,
        bar.width  * 0.155f,
        bar.height * 0.39f
    };
}

Rectangle ShopUI::ShopSlotRect(int i) {
    Rectangle bar = ShopBarRect();

    float left  = bar.width * 0.18f;
    float right = bar.width * 0.99f;
    float gap   = bar.width * 0.007f;
    float cardH = bar.height * 0.86f;

    float stripW = right - left;
    float cardW  = (stripW - 4.0f * gap) / 5.0f;

    return {
        bar.x + left + i * (cardW + gap),
        bar.y + (bar.height - cardH) / 2.0f,
        cardW,
        cardH
    };
}

Rectangle ShopUI::ShopSellRect() {
    Rectangle first = ShopSlotRect(0);
    Rectangle last = ShopSlotRect(4);

    return {
        first.x,
        first.y,
        last.x + last.width - first.x,
        first.height
    };
}

Rectangle ShopUI::ShopProbabilities() {
    Rectangle bar = ShopBarRect();
    Rectangle levelRect = {bar.x, bar.y - bar.height * 0.25f, bar.width * 0.15f, bar.height * 0.25f};

    return {levelRect.x + levelRect.width, levelRect.y + levelRect.height * 0.27f, levelRect.width * 1.70f, levelRect.height * 0.73f};
}

void ShopUI::DrawProbabilities(Engine& engine) {
    Rectangle probability = ShopProbabilities();
    Rectangle firstSlot = ShopSlotRect(0);
    const vector<int>& odds = shopodds[engine.gamestate.level - 2];

    float sectionWidth = (probability.x + probability.width - firstSlot.x) / 5.0f;

    for (int i = 0; i < 5; i++) {
        const char* text = TextFormat("*%d%%", odds[i]);
        Vector2 textSize = MeasureTextEx(traitFont, text, 14.0f, 0.5f);

        float t = i / 4.0f;
        float textX = firstSlot.x + t * ((probability.x + probability.width - textSize.x) - firstSlot.x);

        DrawTextEx(traitFont, text, {textX, probability.y + (probability.height - textSize.y) / 2.0f}, 14.0f, 0.5f, CostTierColor(i + 1));
    }
}

void ShopUI::DrawLock(bool locked) {
    Rectangle lock = LockRect();

    float inset = lock.height * 0.14f;
    Rectangle rect = {lock.x + inset, lock.y + inset, lock.width - inset * 2.0f, lock.height - inset};

    Color topColor = {28, 66, 66, 255};
    Color bottomColor = {22, 35, 35, 255};

    DrawRectangleGradientEx(rect, topColor, bottomColor, bottomColor, topColor);

    inset = rect.height * 0.06f;
    Rectangle innerect = {rect.x + inset, rect.y + inset, rect.width - inset * 2.0f, rect.height - inset * 2.0f};

    DrawRectangleRec(innerect, {14, 20, 23, 255});
}

#include "rlgl.h"

void ShopUI::DrawGold(int gold) {
    Rectangle shop = ShopBarRect();

    float centerX = (shop.x + shop.width / 2.0f) * 1.0925f;
    float height = shop.height * 0.25f;
    float bottomWidth = shop.width * 0.14f;
    float topWidth = bottomWidth * 0.60f;

    Vector2 topLeft = {centerX - topWidth / 2.0f, shop.y - height};
    Vector2 topRight = {centerX + topWidth / 2.0f, shop.y - height};
    Vector2 bottomRight = {centerX + bottomWidth / 2.0f, shop.y};
    Vector2 bottomLeft = {centerX - bottomWidth / 2.0f, shop.y};

    float inset = 5.0f;
    float run = (bottomWidth - topWidth) / 2.0f;
    float slope = run / height;
    float sideOffsetX = inset * sqrtf(1.0f + slope * slope);

    float topY = topLeft.y + inset;
    float bottomY = bottomLeft.y - inset;

    float topHalfWidth = topWidth / 2.0f + slope * inset - sideOffsetX;
    float bottomHalfWidth = bottomWidth / 2.0f - slope * inset - sideOffsetX;

    Vector2 leftTop = {centerX - topHalfWidth, topY};
    Vector2 leftBottom = {centerX - bottomHalfWidth, bottomY};
    Vector2 leftInnerBottom = {leftTop.x, bottomY};

    Vector2 rightTop = {centerX + topHalfWidth, topY};
    Vector2 rightInnerBottom = {rightTop.x, bottomY};
    Vector2 rightBottom = {centerX + bottomHalfWidth, bottomY};

    Rectangle middle = {leftTop.x, leftTop.y, rightTop.x - leftTop.x, bottomY - topY};

    Color topColor = {28, 66, 66, 255};
    Color bottomColor = {22, 35, 35, 255};

    rlBegin(RL_TRIANGLES);

    rlColor4ub(topColor.r, topColor.g, topColor.b, topColor.a);
    rlVertex2f(leftTop.x, leftTop.y);

    rlColor4ub(bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a);
    rlVertex2f(leftBottom.x, leftBottom.y);

    rlColor4ub(bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a);
    rlVertex2f(leftInnerBottom.x, leftInnerBottom.y);

    rlEnd();

    DrawRectangleGradientEx(middle, topColor, bottomColor, bottomColor, topColor);

    rlBegin(RL_TRIANGLES);

    rlColor4ub(topColor.r, topColor.g, topColor.b, topColor.a);
    rlVertex2f(rightTop.x, rightTop.y);

    rlColor4ub(bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a);
    rlVertex2f(rightInnerBottom.x, rightInnerBottom.y);

    rlColor4ub(bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a);
    rlVertex2f(rightBottom.x, rightBottom.y);

    rlEnd();

    float innerInset = 2.0f;

    float innerHeight = leftBottom.y - leftTop.y;
    float innerTopWidth = rightTop.x - leftTop.x;
    float innerBottomWidth = rightBottom.x - leftBottom.x;

    float innerRun = (innerBottomWidth - innerTopWidth) / 2.0f;
    float innerSlope = innerRun / innerHeight;
    float innerSideOffsetX = innerInset * sqrtf(1.0f + innerSlope * innerSlope);

    float innerTopY = leftTop.y + innerInset;
    float innerBottomY = leftBottom.y - innerInset;

    float innerTopHalfWidth = innerTopWidth / 2.0f + innerSlope * innerInset - innerSideOffsetX;
    float innerBottomHalfWidth = innerBottomWidth / 2.0f - innerSlope * innerInset - innerSideOffsetX;

    Vector2 innerTopLeft = {centerX - innerTopHalfWidth, innerTopY};
    Vector2 innerTopRight = {centerX + innerTopHalfWidth, innerTopY};
    Vector2 innerBottomRight = {centerX + innerBottomHalfWidth, innerBottomY};
    Vector2 innerBottomLeft = {centerX - innerBottomHalfWidth, innerBottomY};

    Color innerColor = {14, 20, 23, 255};

    DrawTriangle(innerTopLeft, innerBottomLeft, innerTopRight, innerColor);
    DrawTriangle(innerTopRight, innerBottomLeft, innerBottomRight, innerColor);

    std::string goldText = std::to_string(gold);

    float fontSize = middle.height * 0.60f;
    float circleRadius = fontSize * 0.20f;
    float gap = fontSize * 0.25f;

    Vector2 textSize = MeasureTextEx(uiFont, goldText.c_str(), fontSize, 0.0f);

    float totalWidth = circleRadius * 2.0f + gap + textSize.x;
    float startX = middle.x + (middle.width - totalWidth) / 2.0f;
    float centerY = middle.y + middle.height / 2.0f;

    DrawCircle(startX + circleRadius, centerY, circleRadius, {180, 157, 100, 255});
    DrawTextEx(uiFont, goldText.c_str(), {startX + circleRadius * 2.0f + gap, centerY - textSize.y / 2.0f}, fontSize, 0.0f, WHITE);
}

void ShopUI::DrawXp(bool hoverXp) {
    Rectangle rect = ShopXpRect();
    DrawRectangleRec(rect, BLACK);

    float inset = rect.height * 0.02f;
    Rectangle blackoutline = {rect.x + inset, rect.y + inset, rect.width - inset * 2.0f, rect.height - inset * 2.0f};
    DrawRectangleRec(blackoutline, {86, 123, 141, 255});

    inset = blackoutline.height * 0.02f;
    Rectangle inside = {blackoutline.x + inset, blackoutline.y + inset, blackoutline.width - inset * 2.0f, blackoutline.height - inset * 2.0f};
    DrawRectangleRec(inside, {1,18,29,255});

    // {19,32,42,255}

    inset = inside.height * 0.02f;
    Rectangle buyxppart = {inside.x + inset, inside.y + inset, inside.width - inset * 2.0f, inside.height - inset * 2.0f};
    DrawRectangleRec(buyxppart, {19,32,42,255});

    float textSize = buyxppart.height * 0.30f;
    DrawTextEx(uiFont, "Buy XP", {buyxppart.x + buyxppart.width * 0.05f, buyxppart.y + buyxppart.height * 0.18f}, textSize, 0.0f, WHITE);

    float goldX = buyxppart.x + buyxppart.width * 0.08f;
    float goldY = buyxppart.y + buyxppart.height * 0.73f;
    float goldRadius = buyxppart.height * 0.07f;

    DrawCircle(goldX, goldY, goldRadius, {180, 157, 100, 255});

    float goldTextSize = buyxppart.height * 0.22f;
    DrawTextEx(uiFont, "4", {goldX + goldRadius * 1.8f, goldY - goldTextSize / 2.0f}, goldTextSize, 0.0f, WHITE);

    Rectangle iconRect = { buyxppart.x + buyxppart.width * 0.68f, buyxppart.y, buyxppart.width * 0.32f, buyxppart.height };
    DrawRectangleRec(iconRect, {34, 59, 74, 255});

    float angle = 60.0f * DEG2RAD;
    float triangleWidth = iconRect.height / tanf(angle);

    Vector2 triangleTopLeft = {iconRect.x - triangleWidth, iconRect.y};
    Vector2 triangleTopRight = {iconRect.x, iconRect.y};
    Vector2 triangleBottomRight = {iconRect.x, iconRect.y + iconRect.height};

    DrawTriangle(triangleTopLeft, triangleBottomRight, triangleTopRight, {34, 59, 74, 255});
}

void ShopUI::DrawReroll(bool hoverReroll) {
    Rectangle rect = ShopRerollRect();
    DrawRectangleRec(rect, BLACK);

    float inset = rect.height * 0.02f;
    Rectangle blackoutline = {rect.x + inset, rect.y + inset, rect.width - inset * 2.0f, rect.height - inset * 2.0f};
    DrawRectangleRec(blackoutline, {111, 93, 57, 255});

    inset = blackoutline.height * 0.02f;
    Rectangle inside = {blackoutline.x + inset, blackoutline.y + inset, blackoutline.width - inset * 2.0f, blackoutline.height - inset * 2.0f};
    DrawRectangleRec(inside, {1,18,29,255});

    // {19,32,42,255}

    inset = inside.height * 0.02f;
    Rectangle buyxppart = {inside.x + inset, inside.y + inset, inside.width - inset * 2.0f, inside.height - inset * 2.0f};
    DrawRectangleRec(buyxppart, {31,33,24,255});

    float textSize = buyxppart.height * 0.30f;
    DrawTextEx(uiFont, "Refresh", {buyxppart.x + buyxppart.width * 0.05f, buyxppart.y + buyxppart.height * 0.18f}, textSize, 0.0f, WHITE);

    float goldX = buyxppart.x + buyxppart.width * 0.08f;
    float goldY = buyxppart.y + buyxppart.height * 0.73f;
    float goldRadius = buyxppart.height * 0.07f;

    DrawCircle(goldX, goldY, goldRadius, {180, 157, 100, 255});

    float goldTextSize = buyxppart.height * 0.22f;
    DrawTextEx(uiFont, "2", {goldX + goldRadius * 1.8f, goldY - goldTextSize / 2.0f}, goldTextSize, 0.0f, WHITE);

    Rectangle iconRect = { buyxppart.x + buyxppart.width * 0.68f, buyxppart.y, buyxppart.width * 0.32f, buyxppart.height };
    DrawRectangleRec(iconRect, {70, 64, 43, 255});

    float angle = 60.0f * DEG2RAD;
    float triangleWidth = iconRect.height / tanf(angle);

    Vector2 triangleTopLeft = {iconRect.x - triangleWidth, iconRect.y};
    Vector2 triangleTopRight = {iconRect.x, iconRect.y};
    Vector2 triangleBottomRight = {iconRect.x, iconRect.y + iconRect.height};

    DrawTriangle(triangleTopLeft, triangleBottomRight, triangleTopRight, {70, 64, 43, 255});

}

void ShopUI::DrawShop(Engine& engine, int hoveredShop, bool hoverXp, bool hoverReroll, const DragState& dragState, const Champion* champion) {
    DrawShopVisual();
    DrawXpIndicator(engine.gamestate);
    DrawProbabilities(engine);
    DrawLock(engine.gamestate.shoplocked);
    DrawGold(engine.gamestate.gold);
    DrawXp(hoverXp);
    DrawReroll(hoverReroll);

    // DrawShopTrapezoid();
    //DrawRectangleLinesEx(ShopBarRect(), 3.0f, SKYBLUE);
    // DrawRectangleLinesEx(ShopXpRect(), 3.0f, hoverXp ? YELLOW : SKYBLUE);
    // DrawRectangleLinesEx(ShopRerollRect(), 3.0f, hoverReroll ? YELLOW : SKYBLUE);
    vector<Champion> shop = engine.gamestate.shop;

    for (int i = 0; i < 5; i++) {
        Rectangle rect = ShopSlotRect(i);
        Champion& champion = shop[i];
        Color tierColor = CostTierColor(champion.cost);

        bool sourceHidden = dragState.phase == DragPhase::Dragging &&
                            dragState.payload == DragPayload::Champion &&
                            dragState.source.zone == Zone::Shop &&
                            dragState.source.index == i;

        DrawShopIcon(rect, champion, i == hoveredShop, sourceHidden, engine.gamestate.shopSameChampion[i]);
    }

    if (dragState.phase == DragPhase::Dragging && dragState.payload == DragPayload::Champion && dragState.source.zone == Zone::Shop) {
        Rectangle card = ShopSlotRect(dragState.source.index);
        Vector2 mouse = GetMousePosition();
        Rectangle ghost = {mouse.x - dragState.grabOffset.x, mouse.y - dragState.grabOffset.y, card.width, card.height};

        Champion& champion = shop[dragState.source.index];
        Color tierColor = CostTierColor(champion.cost);

        DrawShopIcon(ghost, champion, true, false, false);
    }

    if (dragState.phase == DragPhase::Dragging &&
        dragState.payload == DragPayload::Champion &&
        (dragState.source.zone == Zone::Bench || dragState.source.zone == Zone::Board)) {

        Rectangle sellRect = ShopSellRect();

        DrawRectangleRec(sellRect, {14, 20, 23, 255});
        Champion sold = *champion;
        int gold = 0;
        if (sold.starLevel == 1) gold = sold.cost;
        else if (sold.starLevel == 2) gold = sold.cost == 1 ? 3 : (sold.cost * 3) - 1;
        else if (sold.starLevel == 3) gold = sold.cost == 1 ? 9 : (sold.cost * 9) - 1;

        const char* text = TextFormat("Sell for %dg", gold);
        int fontSize = 24;
        int textWidth = MeasureText(text, fontSize);

        Vector2 textSize = MeasureTextEx(uiFont, text, fontSize, 1.0f);

        DrawTextEx(uiFont, text, {(float)(sellRect.x + sellRect.width / 2.0f - textSize.x / 2.0f), (float)(sellRect.y + sellRect.height / 2.0f - textSize.y / 2.0f)}, fontSize, 1.0f, {190, 156, 87, 255});
    }
}

bool ShopUI::HandleShopDragPress(std::vector<Champion>& shop, int hoveredShop, Drag& drag) {
    if (hoveredShop == -1 || shop[hoveredShop] == nullChamp) return false;

    Rectangle card = ShopSlotRect(hoveredShop);
    Vector2 mouse = GetMousePosition();
    Vector2 offset = {mouse.x - card.x, mouse.y - card.y};

    drag.BeginDrag(DragPayload::Champion, {Zone::Shop, hoveredShop, -1, -1}, offset);

    return true;
}

bool ShopUI::HandleShopDrop(Engine& engine, Drag& drag) {
    const DragState& state = drag.GetState();

    if (state.source.zone != Zone::Shop) return false;

    float dragDistance = Vector2Distance(state.pressPos, GetMousePosition());
    float clickThreshold = 5.0f;
    float buyDragDistance = ShopSlotRect(state.source.index).width * 0.5f;

    if (dragDistance < clickThreshold || dragDistance >= buyDragDistance) {
        engine.buy(state.source.index);
    }

    return true;
}

void ShopUI::DrawXpIndicator(const GameState& gamestate) {
    int currentXp = gamestate.xp;
    int threshold = levelthresholds[gamestate.level];

    if (threshold == 0) return;

    Rectangle innerRect = InnerShopRect();
    Rectangle innerLevelRect = InnerLevelRect();
    Rectangle xp = ShopXpRect();

    float y = innerLevelRect.y + innerLevelRect.height + (innerRect.y - (innerLevelRect.y + innerLevelRect.height)) * 0.25f;
    float height = (innerRect.y - (innerLevelRect.y + innerLevelRect.height)) * 0.50f;

    int sections = (threshold + 3) / 4;
    float gap = 1.0f;
    float sectionWidth = (xp.width - gap * (sections - 1)) / sections;

    for (int i = 0; i < sections; i++) {
        Rectangle section = {xp.x + i * (sectionWidth + gap), y, sectionWidth, height};
        DrawRectangleRec(section, {24, 65, 90, 255});

        int sectionXp = std::min(4, threshold - i * 4);
        float fill = std::clamp((float)(currentXp - i * 4) / sectionXp, 0.0f, 1.0f);

        DrawRectangleRec({section.x, section.y, section.width * fill, section.height}, {115, 243, 245, 255});
    }

    const char* levelText = TextFormat("Lvl. %d", gamestate.level);
    Vector2 textSize = MeasureTextEx(uiFont, levelText, 20.0f, 1.0f);

    DrawTextEx(uiFont, levelText, {innerLevelRect.x + 6.0f, innerLevelRect.y + (innerLevelRect.height - textSize.y) / 2.0f}, 20.0f, 1.0f, WHITE);

    const char* xpText = TextFormat("%d/%d", currentXp, threshold);
    Vector2 xpTextSize = MeasureTextEx(traitFont, xpText, 13.0f, 1.0f);

    DrawTextEx(traitFont, xpText, {innerLevelRect.x + innerLevelRect.width - xpTextSize.x - 6.0f, innerLevelRect.y + (innerLevelRect.height - xpTextSize.y) / 2.0f}, 13.0f, 1.0f, WHITE);
}

void ShopUI::DrawShopVisual() {
    // probabilities
    Rectangle probability = ShopProbabilities();
    DrawRectangleRec(probability, {20, 28, 29, 180});

    float probTriangleWidth = probability.width * 0.08f;

    DrawTriangle(
        {probability.x + probability.width, probability.y + probability.height},
        {probability.x + probability.width + probTriangleWidth, probability.y + probability.height},
        {probability.x + probability.width, probability.y},
        {20, 28, 29, 180}
    );

    // shop icons / xp and leveling
    Rectangle bar = ShopBarRect();
    Color color = {5, 14, 14, 255};

    DrawRectangleRec(bar, color);

    // level bar
    Rectangle levelRect = { bar.x, bar.y - bar.height * 0.25f, bar.width * 0.15f, bar.height * 0.25f};
    float triangleWidth = bar.width * 0.028f;

    DrawRectangleRec(levelRect, color);

    DrawTriangle(
        {levelRect.x + levelRect.width, levelRect.y + levelRect.height},
        {levelRect.x + levelRect.width + triangleWidth, levelRect.y + levelRect.height},
        {levelRect.x + levelRect.width, levelRect.y},
        color
    );

    // gold trapezoid

    float centerX = (bar.x + bar.width / 2.0f) * 1.0925f;
    float trapHeight = bar.height * 0.25f;
    float bottomWidth = bar.width * 0.14f;
    float topWidth = bottomWidth * 0.60f;

    Vector2 topLeft = {centerX - topWidth / 2.0f, bar.y - trapHeight};
    Vector2 topRight = {centerX + topWidth / 2.0f, bar.y - trapHeight};
    Vector2 bottomRight = {centerX + bottomWidth / 2.0f, bar.y};
    Vector2 bottomLeft = {centerX - bottomWidth / 2.0f, bar.y};

    DrawTriangle(topLeft, bottomLeft, topRight, color);
    DrawTriangle(topRight, bottomLeft, bottomRight, color);

    // lock button
    Rectangle lockrect = LockRect();
    DrawRectangleRec(lockrect, color);

    // inside

    Rectangle innerRect = InnerShopRect();
    DrawRectangleRec(innerRect, {14, 20, 23, 255});
    DrawRectangleLinesEx(innerRect, 2.0f, {22, 35, 35, 255});   

    Rectangle xp = ShopXpRect();
    Rectangle firstSlot = ShopSlotRect(0);
    float lineX = ((xp.x + xp.width) + firstSlot.x) / 2.0f;

    DrawLineEx({lineX, innerRect.y}, {lineX, bar.y + bar.height}, 2.0f, {22, 35, 35, 255});

    Rectangle innerLevelRect = InnerLevelRect();
    float innerTriangleWidth = triangleWidth * (innerLevelRect.height / levelRect.height);

    DrawRectangleRec(innerLevelRect, {15, 26, 27, 255});
    DrawTriangle({innerLevelRect.x + innerLevelRect.width, innerLevelRect.y + innerLevelRect.height}, {innerLevelRect.x + innerLevelRect.width + innerTriangleWidth, innerLevelRect.y + innerLevelRect.height}, {innerLevelRect.x + innerLevelRect.width, innerLevelRect.y}, {15, 26, 27, 255});

    // outlines. 
    Color gold = {111, 93, 57, 255};
    float blackThickness = 6.0f;
    float goldThickness = 3.0f;

    Vector2 trapPoints[] = {
        bottomLeft,
        topLeft,
        topRight,
        bottomRight
    };

    for (int i = 0; i < 3; i++) DrawLineEx(trapPoints[i], trapPoints[i + 1], blackThickness, BLACK);
    for (int i = 0; i < 4; i++) DrawCircleV(trapPoints[i], blackThickness / 2.0f, BLACK);

    for (int i = 0; i < 3; i++) DrawLineEx(trapPoints[i], trapPoints[i + 1], goldThickness, gold);
    for (int i = 0; i < 4; i++) DrawCircleV(trapPoints[i], goldThickness / 2.0f, gold);

    Vector2 points[] = {
        {bar.x, bar.y + bar.height},
        {levelRect.x, levelRect.y},
        {levelRect.x + levelRect.width, levelRect.y},
        {levelRect.x + levelRect.width + triangleWidth, bar.y},
        {lockrect.x, bar.y},
        {lockrect.x, lockrect.y},
        {lockrect.x + lockrect.width, lockrect.y},
        {bar.x + bar.width, bar.y + bar.height}
    };

    int pointCount = sizeof(points) / sizeof(points[0]);

    int shadowLayers = 5;

    for (int layer = shadowLayers; layer >= 1; layer--) {
        float t = blackThickness + layer * 1.0f;
        float alpha = 0.015f * (shadowLayers - layer + 1);

        for (int i = 0; i < pointCount - 1; i++) DrawLineEx(points[i], points[i + 1], t, Fade(BLACK, alpha));
        for (int i = 0; i < pointCount; i++) DrawCircleV(points[i], t / 2.0f, Fade(BLACK, alpha));
    }

    for (int i = 0; i < pointCount - 1; i++) DrawLineEx(points[i], points[i + 1], blackThickness, BLACK);
    for (int i = 0; i < pointCount; i++) DrawCircleV(points[i], blackThickness / 2.0f, BLACK);

    for (int i = 0; i < pointCount - 1; i++) DrawLineEx(points[i], points[i + 1], goldThickness, gold);
    for (int i = 0; i < pointCount; i++) DrawCircleV(points[i], goldThickness / 2.0f, gold);

}