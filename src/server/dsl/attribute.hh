#pragma once

#include "dsl.hh"

#include <vector>

#include "reagent.hh"
#include "stratus.hh"
#include "element.hh"

class reagent;

class subattribute {
        float cache;
        std::shared_ptr<reagent> subreagent;
};

class attribute_layer {
        private:
                std::unordered_map <ns_int_t, std::shared_ptr<std::pair <float, stratus> > > stratum;
                std::unordered_map <ns_int_t, std::shared_ptr<reagent> > reagents;
                std::list <std::shared_ptr<reagent> > reagent_list;
        public:
                
};

