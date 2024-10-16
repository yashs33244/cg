#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL2/SDL2/SDL.h"
#include <GL/glew.h>
#include <cmath>
#include <bits/stdc++.h>

// Function to set SDL and OpenGL attributes
void set_sdl_gl_attributes();
void setupOpenGL(); // Add this declaration

// Bresenham's Line Algorithm
void draw_point(int x, int y);
void draw_line_bresenham(int x0, int y0, int x1, int y1);

// Digital Differential Analyzer (DDA) Line Algorithm
void draw_line_dda(int x0, int y0, int x1, int y1);

// Midpoint Circle Algorithm
void draw_circle_midpoint(int x0, int y0, int radius);


// Super Sampling Anti-Aliasing (SSAA)
void draw_line_ssaa(int x0, int y0, int x1, int y1);
void draw_ellipse(int xc, int yc, int rx, int ry) ;
void draw_polygon(const std::vector<std::pair<int, int>>& vertices);

// Cohen-Sutherland Clipping Algorithm
enum class OutCode { INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8 };

OutCode compute_out_code(int x, int y, int x_min, int y_min, int x_max, int y_max);
void cohen_sutherland_clip(float& x0, float& y0, float& x1, float& y1, int x_min, int y_min, int x_max, int y_max);

#endif // GRAPHICS_H
