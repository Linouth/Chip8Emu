#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#define WIDTH 640
#define HEIGHT 320

double ytogl(int y) {
    // Convert Y screen cooridnate to OpenGL Clipping Area coordinate
    double Py = 2.0/HEIGHT;
    return (y * Py) - 1;
}
double xtogl(int y) {
    // Convert X screen coordinate to OpenGL Clipping Area coordinate
    double Px = 2.0/WIDTH;
    return (y * Px) - 1;
}


void Rect(int x, int y, int size) {
    glBegin(GL_QUADS);
        glVertex2d(xtogl(x), ytogl(y));
        glVertex2d(xtogl(x+size), ytogl(y));
        glVertex2d(xtogl(x+size), ytogl(y+size));
        glVertex2d(xtogl(x), ytogl(y+size));
    glEnd();
}


void RenderArray() {
    uint8_t sprite[] = {
        0b10001000,
        0b10001000,
        0b11111000,
        0b00100000,
        0b00101111,
        0b00001001,
        0b00001001,
        0b00001111
    };
    int height = 8;
    int size = 10;

    int curX, curY = 0;

    for (int i = 1; i <= height; i++) {  // Loop through each row
        uint8_t *row = &sprite[height-i];
        int bits_left = 7;
        curX = 0;

        for (int j = 0; j < 8; j++) {  // Loop through each bit in row (byte)
            if ((*row >> bits_left) & 0x01) {  // If the bit is set
                Rect(curX, curY, 10);
            }
            curX += size;
            bits_left--;
        }
        curY += size;
    }
}


void RenderSceneCB() {
    glClear(GL_COLOR_BUFFER_BIT);

    /* glMatrixMode(GL_PROJECTION); */
    /* glLoadIdentity(); */
    /* gluOrtho2D(-1.0, 1.0, -1.0, 1.0); */

    /* Rect(0, 0, 10); */
    /* Rect(0, 310, 10); */
    /* Rect(630, 0, 10); */
    /* Rect(630, 310, 10); */

    RenderArray();

    glutSwapBuffers();
}


int main(int argc, char *argv[]) {


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chip8");

    glutDisplayFunc(RenderSceneCB);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    /* if (!glewInit()) { */
    /*     fprintf(stderr, "Error: Failed initializing GLEW"); */
    /*     return 1; */
    /* } */

    glutMainLoop();
}

