
#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"

// describes where an object is located
enum class Zone {
    None, // no valid location
    Shop,
    Bench,
    Board,
    Inventory,
    SellArea
};

// describes the object being dragged
enum class DragPayload {
    None,
    Champion,
    Item
};

// current state of mouse
enum class DragPhase {
    Idle,
    Dragging
};

// generic reference to  any location that can involve dragging
struct SlotRef {
    Zone zone = Zone::None;
    int index = -1;
    int row = -1;
    int col = -1;
};

// state of a current drag
struct DragState {
    DragPhase phase = DragPhase::Idle;
    DragPayload payload = DragPayload::None;

    SlotRef source;
    SlotRef holdTarget;

    Vector2 pressPos = {};
    Vector2 grabOffset = {};

    float holdTime = 0.0f;
    bool holdReady = false;
};

class Drag
{
private:
    /* data */
    bool SameSlot(const SlotRef& a, const SlotRef& b) const;
public:
    Drag(/* args */);
    ~Drag();

    DragState state;

    void BeginDrag(DragPayload payload, const SlotRef& source, Vector2 grabOffset = {});
    void ResetDrag();

    bool IsDragging() const;
    bool IsDraggedSource(const SlotRef& slot) const;

    const DragState& getState() const;
    const SlotRef& GetSource() const;
    DragPayload GetPayload() const;

    float getDragDistance() const;

    const DragState& GetState() const;
};