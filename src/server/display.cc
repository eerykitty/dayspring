#include "display.hh"

display::display ()
{
        size_x = 500;
        size_y = 500;

        window = NULL;
        window_surface = NULL;

        if (SDL_Init (SDL_INIT_VIDEO) < 0)
        {
                console::t_error ("SDL", "Awh crap.");
        }
}

display::~display ()
{
        SDL_Quit ();
}

void display::create_window ()
{
        window = SDL_CreateWindow ("Dayspring",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        size_x,
                                        size_y,
                                        SDL_WINDOW_SHOWN);
        if (window == NULL)
                console::t_error ("SDL", "this is not a good day");
}

void display::destroy_window ()
{
        SDL_DestroyWindow (window);
}
