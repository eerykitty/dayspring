#pragma once

#include "dsl.hh"

#include <list>

#include "element.hh"

class stratus {
        private:
                std::list<element> elements;
        public:
                std::list<element>::iterator add (element elem);
                void remove (std::list<element>::iterator elem);
                void modify (std::list<element>::iterator elem, element state);
                float evaluate (float base = 0);
};
