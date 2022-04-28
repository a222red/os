#ifndef EVENT_HPP
#define EVENT_HPP

#include "types.hpp"

enum EventType: u8 {
    KEY_DOWN,
    KEY_UP
};

typedef struct {
    EventType type;
    union {
        u8 scancode;
    } data;
} Event;

// 256 events is probably a bit overkill
Event event_stack[256];
volatile u32 event_index = 0;

static void push_event(const Event& event) {
    const u32 idx = event_index;
    
    if (idx < 255) {
        event_stack[idx] = event;
        event_index = idx + 1;
    }
    else {
        puts("Event stack maxed out");
    }
}

static bool get_event(Event* event) {
    const u32 idx = event_index;
    
    if (idx > 0) {
        event_index = idx - 1;
        *event = event_stack[idx - 1];
        return true;
    }
    else return false;
}

#endif
