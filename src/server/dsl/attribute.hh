#pragma once

#include "dsl.hh"

#include <list>

#include "stratus.hh"
#include "element.hh"

class attribute {
        private:
                std::list <std::shared_ptr<stratus> > strata;
        public:
               int int_evaluate (); 
};

