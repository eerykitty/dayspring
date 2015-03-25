#pragma once

#include "console.hh"

#include "SDL2/SDL.h"

class display
{
        private:
                int size_x;
                int size_y;
                SDL_Window* window;
                SDL_Surface* window_surface;

        public:
                display ();
                ~display ();

                void create_window ();
                void destroy_window ();
};
