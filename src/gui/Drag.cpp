#include <string>
#include <unordered_map>
#include "raylib.h"
#include "../Engine.h"
#include "Drag.h"

Drag::Drag() {
    state = DragState{};
}

Drag::~Drag() {
}


bool Drag::SameSlot(const SlotRef& a, const SlotRef& b) const {
    if (a.zone != b.zone) return false;

    if (a.zone == Zone::Board) {
        return a.row == b.row && a.col == b.col;
    }

    return a.index == b.index;
}

void Drag::BeginDrag(DragPayload payload, const SlotRef& source, Vector2 grabOffset) {
    state.phase = DragPhase::Dragging;
    state.payload = payload;
    state.source = source;
    state.pressPos = GetMousePosition();
    state.grabOffset = grabOffset;

    state.holdTarget = SlotRef{};
    state.holdTime = 0.0f;
    state.holdReady = false;
}

void Drag::ResetDrag() {
    state = DragState{};
}


bool Drag::IsDraggedSource(const SlotRef& slot) const {
    if (state.phase != DragPhase::Dragging) return false;
    return SameSlot(state.source, slot);
}

const DragState& Drag::GetState() const {
    return state;
}