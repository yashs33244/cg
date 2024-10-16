#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>

void setupOpenGL(int width, int height);
void drawTestLine();
void draw_line_bresenham(int x1, int y1, int x2, int y2);
void draw_circle_midpoint(int xc, int yc, int r);

// New functions
void draw_point(int x, int y);
void draw_line_dda(int x1, int y1, int x2, int y2);
void draw_line_midpoint(int x1, int y1, int x2, int y2);
void draw_line_gupta_sproull(int x1, int y1, int x2, int y2);
void draw_circle(int xc, int yc, int r);
void draw_ellipse(int xc, int yc, int rx, int ry);
void draw_ellipse_midpoint(int xc, int yc, int rx, int ry);
void draw_polygon(int* points, int num_points);
void boundary_fill_4(int x, int y, float* fill_color, float* boundary_color);
void boundary_fill_8(int x, int y, float* fill_color, float* boundary_color);
void draw_bitmap(unsigned char* bitmap, int width, int height, int x, int y);
void draw_3d_cube(float size);

// Helper function to replace gluPerspective
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

#endif