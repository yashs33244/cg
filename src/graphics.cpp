#include <GL/glew.h>
#include <iostream>
#include "graphics.h"

// Helper function to check for OpenGL errors
void checkGLError(const char* location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": " << std::hex << err << std::endl;
    }
}

// Setup OpenGL for 2D rendering
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

// Bresenham's line drawing algorithm
void draw_line_bresenham(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glColor3f(1.0f, 0.0f, 0.0f);  // Set the line color to red
    glBegin(GL_POINTS);            // Start drawing points

    while (true)
    {
        glVertex2i(x1, y1);        // Draw the current point
        if (x1 == x2 && y1 == y2)  // Break if we've reached the end point
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    glEnd();                       // End drawing points
    glFlush();                     // Ensure the drawing commands are executed

    checkGLError("draw_line_bresenham");
}

// Midpoint circle drawing algorithm
void draw_circle_midpoint(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    glBegin(GL_POINTS); // Start drawing points

    while (x <= y)
    {
        // Draw the 8 symmetric points
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);

        if (d < 0) {
            d = d + 2 * x + 3;
        } else {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }

    glEnd(); // End drawing points

    checkGLError("draw_circle_midpoint");
}
