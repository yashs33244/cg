#include "graphics.h"
#include <algorithm> // Add this line
#include <cmath>

// Function to set SDL and OpenGL attributes



void setupOpenGL() {
    glViewport(0, 0, 1024, 768);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 768, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to black
}

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

    glBegin(GL_LINES); // Change to GL_LINES
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
    glEnd(); // End the line drawing
}


// Digital Differential Analyzer (DDA) Line Algorithm
void draw_line_dda(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = std::max(abs(dx), abs(dy)); // Ensure <algorithm> is included
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = x0;
    float y = y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

// Updated Midpoint Circle Algorithm
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
    glEnd(); // End the circle drawing
}

// Super Sampling Anti-Aliasing (SSAA)
void draw_line_ssaa(int x0, int y0, int x1, int y1)
{
    for (float i = 0; i <= 1; i += 0.5) {
        for (float j = 0; j <= 1; j += 0.5) {
            draw_line_dda(x0 + i, y0 + j, x1 + i, y1 + j);
        }
    }
}

// Cohen-Sutherland Clipping Algorithm
OutCode compute_out_code(int x, int y, int x_min, int y_min, int x_max, int y_max)
{
    OutCode code = OutCode::INSIDE;
    if (x < x_min) code = static_cast<OutCode>(static_cast<int>(code) | static_cast<int>(OutCode::LEFT));
    if (x > x_max) code = static_cast<OutCode>(static_cast<int>(code) | static_cast<int>(OutCode::RIGHT));
    if (y < y_min) code = static_cast<OutCode>(static_cast<int>(code) | static_cast<int>(OutCode::BOTTOM));
    if (y > y_max) code = static_cast<OutCode>(static_cast<int>(code) | static_cast<int>(OutCode::TOP));
    return code;
}

void cohen_sutherland_clip(float& x0, float& y0, float& x1, float& y1, int x_min, int y_min, int x_max, int y_max)
{
    OutCode code0 = compute_out_code(x0, y0, x_min, y_min, x_max, y_max);
    OutCode code1 = compute_out_code(x1, y1, x_min, y_min, x_max, y_max);
    bool accept = false;

    while (true) {
        if (!(static_cast<int>(code0) | static_cast<int>(code1))) {
            accept = true;
            break;
        } else if (static_cast<int>(code0) & static_cast<int>(code1)) {
            break;
        } else {
            int x, y;
            OutCode code_out = (code0 != OutCode::INSIDE) ? code0 : code1;

            if (static_cast<int>(code_out) & static_cast<int>(OutCode::TOP)) {
                x = x0 + (x1 - x0) * (y_max - y0) / (y1 - y0);
                y = y_max;
            } else if (static_cast<int>(code_out) & static_cast<int>(OutCode::BOTTOM)) {
                x = x0 + (x1 - x0) * (y_min - y0) / (y1 - y0);
                y = y_min;
            } else if (static_cast<int>(code_out) & static_cast<int>(OutCode::RIGHT)) {
                y = y0 + (y1 - y0) * (x_max - x0) / (x1 - x0);
                x = x_max;
            } else if (static_cast<int>(code_out) & static_cast<int>(OutCode::LEFT)) {
                y = y0 + (y1 - y0) * (x_min - x0) / (x1 - x0);
                x = x_min;
            }

            if (code_out == code0) {
                x0 = x;
                y0 = y;
                code0 = compute_out_code(x0, y0, x_min, y_min, x_max, y_max);
            } else {
                x1 = x;
                y1 = y;
                code1 = compute_out_code(x1, y1, x_min, y_min, x_max, y_max);
            }
        }
    }

    if (accept) {
        glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glEnd();
    }
}
