#pragma once

#include "dsl.hh"

enum element_operation {
        EQUAL,
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
        };

// basic element state:
struct element_state {
        int64_t integer;
        std::shared_ptr <std::string> string;
};

struct element {
        element element (element_operation operation, element_state state);
        element_operation operation;
        element_state state;
};
