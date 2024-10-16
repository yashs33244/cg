#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include "graphics.h"
#include <bits/stdc++.h>
using namespace std; 

// Helper function to check for OpenGL errors
void checkGLError(const char* location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": " << std::hex << err << std::endl;
    }
}

// Setup OpenGL for 2D rendering
void setupOpenGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white

    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

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

void draw_point(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void draw_line_dda(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    float x_increment = dx / (float)steps;
    float y_increment = dy / (float)steps;

    float x = x1, y = y1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));
        x += x_increment;
        y += y_increment;
    }
    glEnd();
}

void draw_line_midpoint(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = dy - (dx / 2);
    int x = x1, y = y1;

    glBegin(GL_POINTS);
    glVertex2i(x, y);

    while (x < x2) {
        x++;
        if (d < 0)
            d = d + dy;
        else {
            d += (dy - dx);
            y++;
        }
        glVertex2i(x, y);
    }
    glEnd();
}

void draw_line_gupta_sproull(int x1, int y1, int x2, int y2) {
    // This is a simplified version without anti-aliasing
    draw_line_midpoint(x1, y1, x2, y2);
}

void draw_circle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    glBegin(GL_POINTS);
    while (y >= x) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else
            d = d + 4 * x + 6;
    }
    glEnd();
}

void draw_ellipse(int xc, int yc, int rx, int ry) {
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    glBegin(GL_POINTS);
    while (dx < dy) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);

        if (d1 < 0) {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        } else {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);

    while (y >= 0) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);

        if (d2 > 0) {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
    glEnd();
}

void draw_ellipse_midpoint(int xc, int yc, int rx, int ry) {
    draw_ellipse(xc, yc, rx, ry);  // For simplicity, we're using the same implementation
}

void draw_polygon(int* points, int num_points) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_points; i++) {
        glVertex2i(points[i*2], points[i*2+1]);
    }
    glEnd();
}

void boundary_fill_4(int x, int y, float* fill_color, float* boundary_color) {
    float color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
    
    if ((color[0] != boundary_color[0] || color[1] != boundary_color[1] || color[2] != boundary_color[2]) &&
        (color[0] != fill_color[0] || color[1] != fill_color[1] || color[2] != fill_color[2])) {
        glColor3fv(fill_color);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
        
        boundary_fill_4(x + 1, y, fill_color, boundary_color);
        boundary_fill_4(x - 1, y, fill_color, boundary_color);
        boundary_fill_4(x, y + 1, fill_color, boundary_color);
        boundary_fill_4(x, y - 1, fill_color, boundary_color);
    }
}

void boundary_fill_8(int x, int y, float* fill_color, float* boundary_color) {
    float color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
    
    if ((color[0] != boundary_color[0] || color[1] != boundary_color[1] || color[2] != boundary_color[2]) &&
        (color[0] != fill_color[0] || color[1] != fill_color[1] || color[2] != fill_color[2])) {
        glColor3fv(fill_color);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
        
        boundary_fill_8(x + 1, y, fill_color, boundary_color);
        boundary_fill_8(x - 1, y, fill_color, boundary_color);
        boundary_fill_8(x, y + 1, fill_color, boundary_color);
        boundary_fill_8(x, y - 1, fill_color, boundary_color);
        boundary_fill_8(x + 1, y + 1, fill_color, boundary_color);
        boundary_fill_8(x + 1, y - 1, fill_color, boundary_color);
        boundary_fill_8(x - 1, y + 1, fill_color, boundary_color);
        boundary_fill_8(x - 1, y - 1, fill_color, boundary_color);
    }
}

void draw_bitmap(unsigned char* bitmap, int width, int height, int x, int y) {
    glRasterPos2i(x, y);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, bitmap);
}

void draw_3d_cube(float size)
{
    static float rotation = 0.0f; // Static variable to store rotation angle
    rotation += 0.5f; // Increment rotation angle

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(rotation, 1.0f, 1.0f, 1.0f); // Rotate around all axes
    
    glBegin(GL_QUADS);
    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-size/2, -size/2, size/2);
    glVertex3f(size/2, -size/2, size/2);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(-size/2, size/2, size/2);

    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(size/2, size/2, -size/2);
    glVertex3f(size/2, -size/2, -size/2);

    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(-size/2, size/2, size/2);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(size/2, size/2, -size/2);

    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(size/2, -size/2, -size/2);
    glVertex3f(size/2, -size/2, size/2);
    glVertex3f(-size/2, -size/2, size/2);

    // Right face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(size/2, -size/2, -size/2);
    glVertex3f(size/2, size/2, -size/2);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(size/2, -size/2, size/2);

    // Left face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(-size/2, -size/2, size/2);
    glVertex3f(-size/2, size/2, size/2);
    glVertex3f(-size/2, size/2, -size/2);
    glEnd();

    glPopMatrix();
}