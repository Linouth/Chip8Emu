#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string.h>


struct Chip8State {
    uint8_t     V[16];
    uint16_t    I;
    uint16_t    SP;
    uint16_t    PC;
    uint8_t     delay;
    uint8_t     sound;
    uint8_t     *memory;
    uint8_t     *screen;
    uint8_t     key_state[16];
    uint8_t     save_key_state[16];
    int         waiting_for_key;
};

typedef struct Chip8State Chip8State;

Chip8State* InitChip() {
    Chip8State* s = calloc(sizeof(Chip8State), 1);

    s->memory   = calloc(1024*4, 1);
    s->screen   = &s->memory[0xf00];
    s->SP       = 0xfa0;  // 0xEA0-0xEFF
    s->PC       = 0x200;

    return s;
}

void UnimplementedInstruction(Chip8State *state) {
    printf("Instruction for opcode %04x not implemented\n", state->memory[state->PC]);
}

void Op0(Chip8State *state, uint8_t *op) {
    switch(op[1]) {
        case 0xe0:  // CLEAR
            memset(state->screen, 0, 64*32/8);  // Why /8?
            state->PC += 2;
            break;
        case 0xee:  // return;
            {
                uint16_t target = (state->memory[state->SP] << 8) | state->memory[state->SP + 1];
                state->SP += 2;
                state->PC = target;
            }
            break;
        default:
            UnimplementedInstruction(state);
            break;
    }
}

// goto NNN;
void Op1(Chip8State *state, uint8_t *op) {
    state->PC = ((op[0] & 0x0f) << 8) | op[1];
}

// *(0xNNN)()
void Op2(Chip8State *state, uint8_t *op) {
    state->SP -= 2;
    // state->memory[state->SP] = ((state->PC + 2) & 0xff00) >> 8;
    state->memory[state->SP] = (state->PC + 2) >> 8;            // (PC + 2) for next instruction
    state->memory[state->SP + 1] = (state->PC + 2) & 0xff;
    state->PC = ((op[0] & 0x0f) << 8) | op[1];
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
    uint8_t x = op[0] & 0x0f;
    uint8_t y = op[1] >> 4;
    switch(op[1] & 0x0f) {
        case 0x00:  // Vx=Vy
            state->V[x] = state->V[y];
            break;
        case 0x01:  // Vx=Vx|Vy
            state->V[x] = state->V[x] | state->V[y];
            break;
        case 0x02:  // Vx=Vx&Vy
            state->V[x] = state->V[x] & state->V[y];
            break;
        case 0x03:  // Vx=Vx^Vy
            state->V[x] = state->V[x] ^ state->V[y];
            break;
        case 0x04:  // Vx += Vy
            state->V[x] += state->V[y];
            break;
        case 0x05:  // Vx -= Vy
            state->V[x] -= state->V[y];
            break;
        case 0x06:  // Vx >> 1
            state->V[x] = state->V[x] >> 1;
            break;
        case 0x07:  // Vx=Vy-Vx
            state->V[x] = state->V[y] - state->V[x];
            break;
        case 0x0e:  // Vx << 1
            state->V[x] = state->V[x] << 1;
            break;
    }
    state->PC += 2;
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
    uint8_t reg = op[0] & 0x0f;
    state->V[reg] = random() & op[1];
    state->PC += 2;
}

// draw(Vx,Vy,N)
void OpD(Chip8State *state, uint8_t *op) {  // TODO: ??  Comment...
    int height  = op[1] & 0x0f;
    int x       = state->V[op[0] & 0x0f];
    int y       = state->V[op[1] >> 4];

    state->V[0x0f] = 0;
    for (int i = 0; i < height; i++) {  // Loop through each row
        uint8_t *sprite = &state->memory[state->I+i];
        int spritebit = 7;
        for (int j = x; j < x+8 && j < 64; j++) {  // Loop through sprite row
            int byte_in_row = j / 8;
            int bit_in_byte = j % 8;

            uint8_t srcbit = (*sprite >> spritebit) & 0x01;
            if (srcbit) {
                uint8_t *destbyte_p = &state->screen[(i+y) * (64/8) + byte_in_row];
                uint8_t destbyte = *destbyte_p;
                uint8_t destmask = (0x80 >> bit_in_byte);
                uint8_t destbit = destbyte & destmask;

                srcbit = srcbit << (7-bit_in_byte);

                if (srcbit &destbit) {
                    state->V[0x0f] = 1;
                }

                destbit ^= srcbit;

                destbyte = (destbyte & ~destmask) | destbit;

                *destbyte_p = destbyte;
            }
            spritebit--;
        }
    }

    state->PC += 2;
}

void OpE(Chip8State *state, uint8_t *op) {
    uint8_t reg = op[0] & 0x0f;
    switch(op[1]) {
        case 0x9e:  // if(key()==Vx)
            if (state->key_state[state->V[reg]] != 0) {
                state->PC += 2;
            }
            break;
        case 0xa1:  // if(key()!=Vx)
            if (state->key_state[state->V[reg]] == 0) {
                state->PC += 2;
            }
            break;
    }
    state->PC += 2;
}

void OpF(Chip8State *state, uint8_t *op) {
    uint8_t reg = op[0] & 0x0f;
    switch(op[1]){
        case 0x07:  // Vx = get_delay()
            state->V[reg] = state->delay;
            break;
        case 0x0a:  // Vx = get_key();  Wait until key pressed
            {
                if (state->waiting_for_key == 0) {
                    // Save key_states
                    memcpy(&state->save_key_state, &state->key_state, 16);
                    state->waiting_for_key = 1;
                    return;  // Return without PC++
                } else {
                    for (int i = 0; i < 16; i++) {  // Loop through all 16 keys
                        if ((state->save_key_state[i] == 0) && (state->key_state[i] == 1)) {  // New key pressed
                            state->V[reg] = i;  // Save key to register
                            state->waiting_for_key = 0;
                            state->PC += 2;
                            return;
                        }
                    }
                    return;  // Return without PC++
                }
            }
            break;
        case 0x15:  // delay_timer(Vx)
            state->delay = state->V[reg];
            break;
        case 0x18:  // sound_timer(Vx)
            state->sound = state->V[reg];
            break;
        case 0x1e:  // I += Vx
            state->I += state->V[reg];
            break;
        case 0x29:  // I = sprite_addr[Vx]
            state->I = state->V[reg] * 5;  // *5 for 5 rows
            break;
        case 0x33:  // BCD
            {
                uint8_t Vx = state->V[reg];
                state->memory[state->I+2] = Vx % 10;    // ones
                Vx /= 10;
                state->memory[state->I+1] = Vx % 10;    // tens
                state->memory[state->I] = Vx / 10;      // hundreds
            }
            break;
        case 0x55:  // reg_dump(Vx, &I)
            {
                for (int i = 0; i <= reg; i++) {
                    state->memory[state->I+i] = state->V[i];
                }
            }
            break;
        case 0x65:  // reg_load(Vx, &I)
            {
                for (int i = 0; i <= reg; i++) {
                    state->V[i] = state->memory[state->I+i];
                }
            }
            break;
        default:
            UnimplementedInstruction(state);
            break;
    }
    state->PC += 2;
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
