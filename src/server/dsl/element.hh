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
struct element {
        element ();
        element (element_operation operation, float state);

        element_operation operation;
        float num;
};
