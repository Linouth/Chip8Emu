#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>


void RenderSceneCB();


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(640, 320);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chip8");

    glutDisplayFunc(RenderSceneCB);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glutMainLoop();
}


void RenderSceneCB() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}
