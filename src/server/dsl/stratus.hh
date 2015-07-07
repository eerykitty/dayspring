#pragma once

#include "dsl.hh"

#include <list>

#include "element.hh"

class stratus {
        private:
                element_state cache;
                std::list<element> elements;
        public:
                stratus ();
                std::list<element>::iterator add (element_operation op, element_state state);
                int64_t evaluate ();
                std::string evaluate ();

                bool dirty;
};
