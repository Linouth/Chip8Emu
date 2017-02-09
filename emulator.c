#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


struct Chip8State {
    uint8_t     V[16];
    uint16_t    I;
    uint16_t    SP;
    uint16_t    PC;
    uint8_t     delay;
    uint8_t     sound;
    uint8_t     *memory;
    uint8_t     *screen;
};

typedef struct Chip8State Chip8State;

Chip8State* InitChip() {
    Chip8State* s = calloc(sizeof(Chip8State), 1);

    s->memory   = calloc(1024*4, 1);
    s->screen   = &s->memory[0xf00];
    s->SP       = 0xea0;  //0xfa0
    s->PC       = 0x200;

    return s;
}

void UnimplementedInstruction(Chip8State *state) {
    printf("Instruction for opcode %04x not implemented", state->memory[state->PC]);
}

void Op0(Chip8State *state, uint8_t *op) {
    
}

// goto NNN;
void Op1(Chip8State *state, uint8_t *op) {
    uint16_t target = ((op[0] & 0x0f) << 8) | op[1];
    state->PC = target;
}

// *(0xNNN)()
void Op2(Chip8State *state, uint8_t *op) {

}

// if(Vx==NN)
void Op3(Chip8State *state, uint8_t *op) {
    uint8_t reg = op[0] & 0x0f;
    if (state->V[reg] == op[1]) {
        state->PC += 4;
    } else {
        state->PC += 2;
    }
}

// if(Vx!=NN)
void Op4(Chip8State *state, uint8_t *op) {
    uint8_t reg = op[0] & 0x0f;
    if (state->V[reg] != op[1]) {
        state->PC += 4;
    } else {
        state->PC += 2;
    }
}

// if(Vx==Vy)
void Op5(Chip8State *state, uint8_t *op) {
    uint8_t regx = op[0] & 0x0f;
    uint8_t regy = op[1]  >> 4;
    if (state->V[regx] == state->V[regy]) {
        state->PC += 4;
    } else {
        state->PC += 2;
    }
}

// Vx = NN
void Op6(Chip8State *state, uint8_t *op) {
    uint8_t reg = op[0] & 0x0f;
    state->V[reg] = op[1];
    state->PC += 2;
}

// Vx += NN
void Op7(Chip8State *state, uint8_t *op) {
    uint8_t reg = op[0] & 0x0f;
    state->V[reg] += op[1];
    state->PC += 2;
}

void Op8(Chip8State *state, uint8_t *op) {
    
}

// if(Vx!=Vy)
void Op9(Chip8State *state, uint8_t *op) {
    uint8_t regx = op[0] & 0x0f;
    uint8_t regy = op[1] >> 4;
    if (state->V[regx] != state->V[regy]) {
        state->PC += 4;
    } else {
        state->PC += 2;
    }
}

// I = NNN
void OpA(Chip8State *state, uint8_t *op) {
    uint16_t dat = ((op[0] & 0x0f) << 8) | op[1];
    state->I = dat;
    state->PC += 2;
}

// PC=V0+NNN
void OpB(Chip8State *state, uint8_t *op) {
    uint16_t dat = ((op[0] & 0x0f) << 8) | op[1];
    state->PC = state->V[0] + dat;
}

// Vx=rand()&NN
void OpC(Chip8State *state, uint8_t *op) {

}

// draw(Vx,Vy,N)
void OpD(Chip8State *state, uint8_t *op) {

}

void OpE(Chip8State *state, uint8_t *op) {

}

void OpF(Chip8State *state, uint8_t *op) {

}

void EmulateChip8Op(Chip8State *state) {
    uint8_t *op = &state->memory[state->PC];

    // int highnib = (*op & 0xf0) >> 4;
    int highnib = *op >> 4;
    switch(highnib) {
        case 0x00: UnimplementedInstruction(state); break;
        case 0x01: Op1(state, op); break;
        case 0x02: UnimplementedInstruction(state); break;
        case 0x03: UnimplementedInstruction(state); break;
        case 0x04: UnimplementedInstruction(state); break;
        case 0x05: UnimplementedInstruction(state); break;
        case 0x06: UnimplementedInstruction(state); break;
        case 0x07: UnimplementedInstruction(state); break;
        case 0x08: UnimplementedInstruction(state); break;
        case 0x09: UnimplementedInstruction(state); break;
        case 0x0a: UnimplementedInstruction(state); break;
        case 0x0b: UnimplementedInstruction(state); break;
        case 0x0c: UnimplementedInstruction(state); break;
        case 0x0d: UnimplementedInstruction(state); break;
        case 0x0e: UnimplementedInstruction(state); break;
        case 0x0f: UnimplementedInstruction(state); break;
    }
}

int main(void) {
    Chip8State *state = InitChip();

    EmulateChip8Op(state);

    return 0;
}
