#include <GL/glew.h>
#include <SDL2/SDL2/SDL.h>
#include <iostream>

SDL_Window* window;
SDL_GLContext gl_context;
bool is_game_running;

void checkGLError(const char* location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": " << std::hex << err << std::endl;
    }
}

void setupOpenGL()
{
    glMatrixMode(GL_PROJECTION);   // Set the projection matrix
    glLoadIdentity();              // Reset the projection matrix
    gluOrtho2D(0, 1024, 0, 768);   // Set orthographic projection (left, right, bottom, top)

    glMatrixMode(GL_MODELVIEW);    // Switch to modelview matrix
    glLoadIdentity();              // Reset the modelview matrix

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white

    glViewport(0, 0, 1024, 768);   // Set the viewport size to match the window

    checkGLError("setupOpenGL");
}

void drawTestLine()
{
    glColor3f(1.0f, 0.0f, 0.0f);  // Set the line color to red

    glBegin(GL_LINES);
    glVertex2i(100, 100);         // Line start point (inside window)
    glVertex2i(300, 300);         // Line end point
    glEnd();

    checkGLError("drawTestLine");

    glFlush();                    // Ensure all drawing commands are executed
}

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

            // Draw the test line
            drawTestLine();

            SDL_GL_SwapWindow(window);    // Swap the buffers to display
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
