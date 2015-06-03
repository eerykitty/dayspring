#pragma once

#include <iostream>
#include <string>

#include "pegtl.hh"

// agent, carries state in the form of attributes.
// attributes are scalar values associated with a hash.
// agents can also carry reagents in a number of varying forms
// reagents can be possessed intrinsicly
// reagents can be possessed as skills 


/*
 * ATTRIBUTES
 *
 * Attributes resolve to simple scalar values, however attributes aren't technically scalar values - they're composites, doubly linked lists of different elements that are resolved by different functions as different interpretations.
 *
 * Triggers - actions which are triggered by attributes reaching a certain condition (less than / equal to / greater than).
 *
 * Actions - change attributes 
 *
 */

class agent
{

};
