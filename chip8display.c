#include <GL/glew.h>
#include <GL/glut.h>

#include "chip8emu.h"

void initWindow(int *argc, char *argv[]) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(640, 320);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chip8");


    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void render(Chip8State *state, uint8_t *buffer) {
    uint8_t *screen_pointer = state->screen;
    uint8_t *tbuf = buffer;
    uint8_t byte;
    for (int i = 0; i < ((64/8)*32); i++) {     // Loop through each byte
        byte = screen_pointer[0];
        if (byte & 0b10000000) tbuf[0] = 0xff; else tbuf[0] = 0x00;
        if (byte & 0b01000000) tbuf[1] = 0xff; else tbuf[1] = 0x00;
        if (byte & 0b00100000) tbuf[2] = 0xff; else tbuf[2] = 0x00;
        if (byte & 0b00010000) tbuf[3] = 0xff; else tbuf[3] = 0x00;
        if (byte & 0b00001000) tbuf[4] = 0xff; else tbuf[4] = 0x00;
        if (byte & 0b00000100) tbuf[5] = 0xff; else tbuf[5] = 0x00;
        if (byte & 0b00000010) tbuf[6] = 0xff; else tbuf[6] = 0x00;
        if (byte & 0b00000001) tbuf[7] = 0xff; else tbuf[7] = 0x00;
        tbuf += 8;
        screen_pointer++;                       // Next byte
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 64, 32, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(-1.0, -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex2f(1.0, -1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex2f(1.0, 1.0);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(-1.0, 1.0);
    glEnd();

    glFlush();
}
