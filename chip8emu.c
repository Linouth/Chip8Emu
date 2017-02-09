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

void Op1(Chip8State *state, uint8_t *op) {
    uint16_t target = ((op[0] & 0x0f) << 8) | op[1];
    state->PC = target;
}

void Op2(Chip8State *state, uint8_t *op) {

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
