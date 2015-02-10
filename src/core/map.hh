#pragma once
#include <unordered_map>
#include <utility>

#include "utility.hh"
#include "item.hh"
#include "entity.hh"
#include "geometry.hh"

/*
 * cells are used to store information about the game world; they can be tiled "indefinitely" (with
 * infinite memory and computer power heh), and have a default size of 500 by 500 tiles.
 */
class cell
{
        private:
                int x;
                int y;
                std::unordered_map <std::pair <int, int>, item>       items;
                std::unordered_map <std::pair <int, int>, entity>     entities;
                std::unordered_map <std::pair <int, int>, geometry>   geometries;

        public:
                cell();
};
