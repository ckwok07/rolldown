#include "TeambuilderUI.h"
#include "rlgl.h"
#include "raymath.h"

TeambuilderUI::TeambuilderUI(/* args */)
{
}

TeambuilderUI::~TeambuilderUI()
{
}

void TeambuilderUI::init() {

}

void TeambuilderUI::shutdown() {

}

void TeambuilderUI::drawTeambuilder() {
    if (!open) {
        drawClose();
    } else {
        drawOpen();
    }
}

void TeambuilderUI::makeOpen() {

}

void TeambuilderUI::drawClose() {
    int height = GetScreenHeight();
    int width = GetScreenWidth();
    float x = width * 0.8385f;

    DrawRectangleRec({x, 00.f, 47.0f, 41.5f}, Fade(BLACK, 0.85f));
    DrawTriangle({x, 0.0f}, {x - 20.0f, 0.0f}, {x, 42.0f}, Fade(BLACK, 0.85f));

    float inset = 1.5f;
    // DrawRectangleRec({x + inset / 2.0f, 0.0f, 46.0f - inset / 2.0f, 40.0f - inset}, {111, 93, 57, 255});
    // DrawTriangle({x + inset / 2.0f, 0.0f}, {x - 20.0f + inset, 0.0f}, {x + inset / 2.0f, 46.0f - inset}, {111, 93, 57, 255}); //{12, 26, 27, 255}
    DrawRectangleRec({x + inset / 2.0f, 0.0f, 47.0f - inset / 2.0f, 41.5f - inset}, {111, 93, 57, 255});
    DrawTriangle({x + inset / 2.0f, 0.0f}, {x - 20.0f + inset, 0.0f}, {x + inset / 2.0f, 41.5f - inset}, {111, 93, 57, 255});

    inset = 5.0f;
    DrawRectangleRec({x + inset / 2.0f, 0.0f, 47.0f - inset / 2.0f, 42.0f - inset}, {12, 26, 27, 255});
    DrawTriangle({x + inset / 2.0f, 0.0f}, {x - 20.0f + inset, 0.0f}, {x + inset / 2.0f, 42.0f - inset}, {12, 26, 27, 255});

    float gradientInset = 3.0f;
    float baseTopLeftX = x - 20.0f + inset, baseBottomLeftX = x + inset / 2.0f, baseBottomY = 42.0f - inset;
    float slope = (baseBottomLeftX - baseTopLeftX) / baseBottomY, sideOffsetX = gradientInset * sqrtf(1.0f + slope * slope);
    float topY = 0.0f, bottomY = baseBottomY - gradientInset;
    float leftTopX = baseTopLeftX + sideOffsetX, leftBottomX = baseTopLeftX + slope * bottomY + sideOffsetX;
    Color bottomColor = {28, 66, 66, 255}, topColor = {22, 35, 35, 255};

    DrawRectangleGradientEx({leftBottomX, topY, x + 47.0f - leftBottomX, bottomY - topY}, topColor, bottomColor, bottomColor, topColor);
    rlBegin(RL_TRIANGLES);
    rlColor4ub(topColor.r, topColor.g, topColor.b, topColor.a); rlVertex2f(leftBottomX, topY);
    rlColor4ub(topColor.r, topColor.g, topColor.b, topColor.a); rlVertex2f(leftTopX, topY);
    rlColor4ub(bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a); rlVertex2f(leftBottomX, bottomY);
    rlEnd();

    inset = 2.0f;
    float offset = inset * sqrtf(1.0f + slope * slope);
    float tY = topY, bY = bottomY - inset, tX = leftTopX + offset, bX = leftBottomX - slope * inset + offset;
    float rightX = x + 47.0f - inset / 2.0f;

    DrawRectangleRec({bX, tY, rightX - bX, bY - tY}, {12, 26, 27, 255});
    DrawTriangle({bX, tY}, {tX, tY}, {bX, bY}, {12, 26, 27, 255});

    // options
    int widthR = GetScreenWidth();
    float xR = widthR * 0.8385f + 47.0f;

    DrawRectangleRec({xR, 0.0f, 47.0f, 41.5f}, Fade(BLACK, 0.85f));
    DrawTriangle({xR + 47.0f, 0.0f}, {xR + 47.0f, 42.0f}, {xR + 67.0f, 0.0f}, Fade(BLACK, 0.85f));

    float insetR = 1.5f;
    DrawRectangleRec({xR, 0.0f, 47.0f - insetR / 2.0f, 41.5f - insetR}, {111, 93, 57, 255});
    DrawTriangle({xR + 47.0f - insetR / 2.0f, 0.0f}, {xR + 47.0f - insetR / 2.0f, 41.5f - insetR}, {xR + 67.0f - insetR, 0.0f}, {111, 93, 57, 255});

    insetR = 5.0f;
    DrawRectangleRec({xR, 0.0f, 47.0f - insetR / 2.0f, 42.0f - insetR}, {12, 26, 27, 255});
    DrawTriangle({xR + 47.0f - insetR / 2.0f, 0.0f}, {xR + 47.0f - insetR / 2.0f, 42.0f - insetR}, {xR + 67.0f - insetR, 0.0f}, {12, 26, 27, 255});

    float gradientInsetR = 3.0f;
    float baseTopRightXR = xR + 67.0f - insetR, baseBottomRightXR = xR + 47.0f - insetR / 2.0f, baseBottomYR = 42.0f - insetR;
    float slopeR = (baseTopRightXR - baseBottomRightXR) / baseBottomYR, sideOffsetXR = gradientInsetR * sqrtf(1.0f + slopeR * slopeR);
    float topYR = 0.0f, bottomYR = baseBottomYR - gradientInsetR;
    float rightTopXR = baseTopRightXR - sideOffsetXR, rightBottomXR = baseTopRightXR - slopeR * bottomYR - sideOffsetXR;
    Color bottomColorR = {28, 66, 66, 255}, topColorR = {22, 35, 35, 255};

    DrawRectangleGradientEx({xR, topYR, rightBottomXR - xR, bottomYR - topYR}, topColorR, bottomColorR, bottomColorR, topColorR);
    rlBegin(RL_TRIANGLES);
    rlColor4ub(topColorR.r, topColorR.g, topColorR.b, topColorR.a); rlVertex2f(rightBottomXR, topYR);
    rlColor4ub(bottomColorR.r, bottomColorR.g, bottomColorR.b, bottomColorR.a); rlVertex2f(rightBottomXR, bottomYR);
    rlColor4ub(topColorR.r, topColorR.g, topColorR.b, topColorR.a); rlVertex2f(rightTopXR, topYR);
    rlEnd();

    insetR = 2.0f;
    float offsetR = insetR * sqrtf(1.0f + slopeR * slopeR);
    float tYR = topYR, bYR = bottomYR - insetR, tXR = rightTopXR - offsetR, bXR = rightBottomXR + slopeR * insetR - offsetR;
    float leftXR = xR + insetR / 2.0f;

    DrawRectangleRec({leftXR, tYR, bXR - leftXR, bYR - tYR}, {12, 26, 27, 255});
    DrawTriangle({bXR, tYR}, {bXR, bYR}, {tXR, tYR}, {12, 26, 27, 255});
}

void TeambuilderUI::drawOpen() {

}

void TeambuilderUI::drawUnselected() {

}

void TeambuilderUI::drawSelected() {

}