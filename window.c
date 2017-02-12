#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>


void RenderSceneCB() {
    glClear(GL_COLOR_BUFFER_BIT);

    /* glBegin(GL_TRIANGLES); */
    /*     glVertex3f(-0.5, -0.5, 0.0); */
    /*     glVertex3f(0.5, 0.0, 0.0); */
    /*     glVertex3f(0.0, 0.5, 0.0); */
    /* glEnd(); */

    glBegin(GL_QUADS);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
    glEnd();

    glutSwapBuffers();
}


int main(int argc, char *argv[]) {


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(640, 320);
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

