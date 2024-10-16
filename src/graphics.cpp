#include "graphics.h"
#include <algorithm> // Add this line
#include <cmath>

void set_sdl_gl_attributes()
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

// Define the missing setupOpenGL function
// void setupOpenGL()
// {
//     glViewport(0, 0, 1024, 768);  // Set the viewport size
//     glMatrixMode(GL_PROJECTION);   // Switch to the projection matrix
//     glLoadIdentity();              // Reset the projection matrix
    
//     // Use glOrtho to create an orthographic projection
//     glOrtho(0, 1024, 0, 768, -1.0, 1.0); // Left, Right, Bottom, Top, Near, Far

//     glMatrixMode(GL_MODELVIEW);    // Switch back to the model-view matrix
//     glLoadIdentity();              // Reset the model-view matrix
//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color
// }
// Bresenham's Line Algorithm
void draw_line_bresenham(int x0, int y0, int x1, int y1)
{
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int err = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    glBegin(GL_LINES);
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            glVertex2i(y, x);
        } else {
            glVertex2i(x, y);
        }
        err -= dy;
        if (err < 0) {
            y += ystep;
            err += dx;
        }
    }
    glEnd();
}

// Midpoint Circle Algorithm
void draw_circle_midpoint(int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int p = 1 - radius;

    glBegin(GL_POINTS);
    while (x >= y) {
        glVertex2i(x0 + x, y0 + y);
        glVertex2i(x0 - x, y0 + y);
        glVertex2i(x0 + x, y0 - y);
        glVertex2i(x0 - x, y0 - y);
        glVertex2i(x0 + y, y0 + x);
        glVertex2i(x0 - y, y0 + x);
        glVertex2i(x0 + y, y0 - x);
        glVertex2i(x0 - y, y0 - x);
        y++;
        if (p <= 0) {
            p += 2 * y + 1;
        } else {
            x--;
            p += 2 * (y - x) + 1;
        }
    }
    glEnd();
}
