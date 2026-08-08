#include "ShopUI.h"
#include "Drag.h"

ShopUI::ShopUI(/* args */)
{
}

ShopUI::~ShopUI()
{
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

void ShopUI::DrawShopIcon(Rectangle rect, Champion& champion, Color tierColor, bool highlighted, bool sourceHidden) {
    float border = 3.0f;
    float infoHeight = rect.height * 0.18f;

    Rectangle innerRect = { rect.x + border, rect.y + border, rect.width - border * 2.0f, rect.height - border * 2.0f};

    // spash art rectangle
    Rectangle artRect = {innerRect.x, innerRect.y, innerRect.width, innerRect.height - infoHeight};

    // name rectangle
    Rectangle infoRect = {innerRect.x, artRect.y + artRect.height, innerRect.width, infoHeight};

    if (champion.id != 0 && !sourceHidden) {
        Texture2D* splash = GetChampionSplash(champion.name);

        if (splash != nullptr) {
            DrawTextureCover(*splash, artRect, WHITE);
        }

        int traitFontSize = 10;
        float traitSpacing = 25.0f;

        float hexRadius = 7.60f;
        float gap = 4.0f;

        float traitY = artRect.y + artRect.height - champion.traits.size() * traitSpacing;

        for (int j = 0; j < champion.traits.size(); j++) {
            float y = traitY + j * traitSpacing;

            Vector2 hexCenter = {artRect.x + gap + hexRadius, y + traitFontSize / 2.0f};
            DrawPoly(hexCenter, 6, hexRadius, 30.0f, GRAY);

            float traitX = hexCenter.x + hexRadius + gap;
            string traitName = Set18::TRAIT_ID_TO_NAME.at(champion.traits[j]);
            DrawText(traitName.c_str(), (int)traitX, (int)y * 1.003, traitFontSize, WHITE);
        }

        DrawRectangleRec(infoRect,tierColor);

        int fontSize = (int)(infoRect.height * 0.60f);
        int textY = (int)(infoRect.y + (infoRect.height - fontSize)/ 1.5);

        // name
        DrawText(champion.name.c_str(), (int)(infoRect.x + 6.0f), textY, fontSize, WHITE );

        // cost
        const char* costText = TextFormat("%d", champion.cost);

        int costWidth =MeasureText(costText, fontSize);

        DrawText( costText,(int)(infoRect.x + infoRect.width - costWidth -6.0f), textY, fontSize, WHITE);
    }

    if (champion.id != 0 && !sourceHidden) {
        if (highlighted) {
            DrawRectangleRec(rect, Fade(WHITE, 0.25f));
        }

        DrawRectangleLinesEx(rect, 3.0f, tierColor);
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

void ShopUI::DrawShop(vector<Champion>& shop, int hoveredShop, bool hoverXp, bool hoverReroll, const DragState& dragState, const Champion* champion) {
    DrawShopTrapezoid();
    DrawRectangleLinesEx(ShopBarRect(), 3.0f, SKYBLUE);
    DrawRectangleLinesEx(ShopXpRect(), 3.0f, hoverXp ? YELLOW : SKYBLUE);
    DrawRectangleLinesEx(ShopRerollRect(), 3.0f, hoverReroll ? YELLOW : SKYBLUE);

    for (int i = 0; i < 5; i++) {
        Rectangle rect = ShopSlotRect(i);
        Champion& champion = shop[i];
        Color tierColor = CostTierColor(champion.cost);

        bool sourceHidden = dragState.phase == DragPhase::Dragging &&
                            dragState.payload == DragPayload::Champion &&
                            dragState.source.zone == Zone::Shop &&
                            dragState.source.index == i;

        DrawShopIcon(rect, champion, tierColor, i == hoveredShop, sourceHidden);
    }

    if (dragState.phase == DragPhase::Dragging && dragState.payload == DragPayload::Champion && dragState.source.zone == Zone::Shop) {
        Rectangle card = ShopSlotRect(dragState.source.index);
        Vector2 mouse = GetMousePosition();
        Rectangle ghost = {mouse.x - dragState.grabOffset.x, mouse.y - dragState.grabOffset.y, card.width, card.height};

        Champion& champion = shop[dragState.source.index];
        Color tierColor = CostTierColor(champion.cost);

        DrawShopIcon(ghost, champion, tierColor, true, false);
    }

    if (dragState.phase == DragPhase::Dragging &&
        dragState.payload == DragPayload::Champion &&
        (dragState.source.zone == Zone::Bench || dragState.source.zone == Zone::Board)) {

        Rectangle sellRect = ShopSellRect();

        DrawRectangleRec(sellRect, Fade(BLACK, 0.75f));
        Champion sold = *champion;
        int gold = 0;
        if (sold.starLevel == 1) gold = sold.cost;
        else if (sold.starLevel == 2) gold = sold.cost == 1 ? 3 : (sold.cost * 3) - 1;
        else if (sold.starLevel == 3) gold = sold.cost == 1 ? 9 : (sold.cost * 9) - 1;

        const char* text = TextFormat("SELL FOR %d GOLD", gold);
        int fontSize = 24;
        int textWidth = MeasureText(text, fontSize);

        DrawText(text, (int)(sellRect.x + sellRect.width / 2.0f - textWidth / 2.0f), (int)(sellRect.y + sellRect.height / 2.0f - fontSize / 2.0f), fontSize, WHITE);
    }
}