#pragma once

#include "dsl.hh"

#include <unordered_map>

#include "agent.hh"
#include "effect.hh"
#include "agent_slot.hh"
#include "trigger.hh"
#include "action.hh"

class reagent {
        std::unordered_map <ns_int_t, std::shared_ptr<reagent> > reagents;
        std::unordered_map <ns_int_t, std::shared_ptr<effect> > effects;
        std::unordered_map <ns_int_t, std::shared_ptr<agent_slot> > agent_slots;
        std::unordered_map <ns_int_t, std::shared_ptr<trigger> > triggers;
        std::unordered_map <ns_int_t, std::shared_ptr<action> > actions;
};
