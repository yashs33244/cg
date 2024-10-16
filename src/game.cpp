#include "graphics.h"
#include <GL/glew.h>
#include <SDL2/SDL2/SDL.h>
#include <iostream>

SDL_Window* window;
SDL_GLContext gl_context;
bool is_game_running;

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL2 initialized successfully." << std::endl;

        window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
        gl_context = SDL_GL_CreateContext(window);

        if (glewInit() == GLEW_OK)
        {
            std::cout << "GLEW initialization successful" << std::endl;
        }
        else
        {
            std::cout << "GLEW initialization failed" << std::endl;
            return -1;
        }

        setupOpenGL();

        SDL_Event event;
        is_game_running = true;

        while (is_game_running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    is_game_running = false;
            }

            glClear(GL_COLOR_BUFFER_BIT); // Clear the screen to the background color

            // Draw the test line and other shapes
            draw_line_bresenham(100, 100, 300, 300);  // Example of a red Bresenham line
            draw_circle_midpoint(400, 300, 100);      // Draw a red circle

            SDL_GL_SwapWindow(window);    // Swap the buffers to display the content
        }

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    else
    {
        std::cout << "SDL2 initialization failed" << std::endl;
        return -1;
    }

    return 0;
}
