#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void DisassembleChip8(uint8_t *codebuffer, int pc) {
    uint8_t *code = &codebuffer[pc];
    int firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);
    switch(firstnib) {
        case 0x00: {
                        switch(code[1]) {
                            case 0xe0: printf("  CLEAR"); break;
                            case 0xee: printf("  RETURN"); break;
                            default: break;
                        }
                   }
                   break;
        case 0x01: {
                       uint8_t dat = code[0] & 0x0f;
                       printf("  goto %1x%02x", dat, code[1]);
                   }
                   break;
        case 0x02: {
                       uint8_t dat = code[0] & 0x0f;
                       printf("  *(0x%1x%02x)", dat, code[1]);
                   }
                   break;
        case 0x03: {
                       uint8_t reg = code[0] & 0x0f;
                       printf("  if(V%1x==0x%02x)", reg, code[1]);
                   }
                   break;
        case 0x04: {
                       uint8_t reg = code[0] & 0x0f;
                       printf("  if(V%1x!=0x%02x)", reg, code[1]);
                   }
                   break;
        case 0x05: printf("5 not handled yet", firstnib); break;
        case 0x06: {
                       uint8_t reg = code[0] & 0x0f;
                       printf("  V%1x = 0x%02x", reg, code[1]);
                   }
                   break;
        case 0x07: {
                       uint8_t reg = code[0] & 0x0f;
                       printf("  V%1x += 0x%02x", reg, code[1]);
                   }
                   break;
        case 0x08: {
                       uint8_t Vx = code[0] & 0x0f;
                       uint8_t Vy = (code[1] >> 4);
                       uint8_t n = code[1] & 0x0f;
                       switch(n) {
                           case 0x1: printf("  V%1x=V%1x|V%1x", Vx, Vx, Vy); break;
                           case 0x2: printf("  V%1x=V%1x&V%1x", Vx, Vx, Vy); break;
                           case 0x3: printf("  V%1x=V%1x^V%1x", Vx, Vx, Vy); break;
                           case 0x4: printf("  V%1x += V%1x", Vx, Vy); break;
                           case 0x5: printf("  V%1x -= V%1x", Vx, Vy); break;
                           case 0x6: printf("  V%1x >> 1", Vx); break;
                           case 0x7: printf("  V%1x=V%1x-V%1x", Vx, Vx, Vy); break;
                           case 0xe: printf("  V%1x << 1", Vx); break;
                       }
                   }
                   break;
        case 0x09: {
                       uint8_t Vx = code[0] & 0x0f;
                       uint8_t Vy = (code[1] >> 4);
                       printf("  if(V%1x!=V%1x)", Vx, Vy);
                   }
                   break;
        case 0x0a: {
                       uint8_t dat = code[0] & 0x0f;
                       printf("  I  = 0x%1x%2x", dat, code[1]);
                   }
                   break;
        case 0x0b: printf("b not handled yet"); break;
        case 0x0c: {
                       uint8_t Vx = code[0] & 0x0f;
                       printf("  V%1x=rand()&%02x", Vx, code[1]);
                   }
                   break;
        case 0x0d: {
                       uint8_t Vx = code[0] & 0x0f;
                       uint8_t Vy = (code[1] >> 4);
                       uint8_t n = code[1] & 0x0f;
                       printf("  draw(0x%1x,0x%1x,0x%1x)", Vx, Vy, n);
                   }
                   break;
        case 0x0e: {
                       uint8_t Vx = code[0] & 0x0f;
                       switch(code[1]) {
                           case 0x9e: printf("  if(key()==V%1x)", Vx); break;
                           case 0xa1: printf("  if(key()!=V%1x)", Vx); break;
                       }
                   }
                   break;
        case 0x0f: {
                       uint8_t x = code[0] & 0x0f;
                       switch(code[1]) {
                           case 0x07: printf("  V%1x = get_delay()", x); break;
                           case 0x0a: printf("  V%1x = get_key()", x); break;
                           case 0x15: printf("  delay_timer(V%1x)", x); break;
                           case 0x18: printf("  sound_timer(V%1x)", x); break;
                           case 0x1e: printf("  I += V%1x", x); break;
                           case 0x29: printf("  I=sprite_addr[V%1x]", x); break;
                           case 0x33: printf("  set_BCD(V%1x)", x); break;
                           case 0x55: printf("  reg_dump(V%1x, &I)", x); break;
                           case 0x65: printf("  reg_load(V%1x, &I)", x); break;
                       }
                   }
                   break;
    }
}

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "rb");
    if (f==NULL) {
        printf("error: Could not open %s\n", argv[1]);
        exit(1);
    }
    
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    //fseek(f, 0L, SEEK_SET);
    rewind(f);

    unsigned char *buffer = malloc(fsize+0x200);
    fread(buffer+0x200, fsize, 1, f);
    fclose(f);

    int pc = 0x200;
    while (pc < (fsize+0x200)) {
        DisassembleChip8(buffer, pc);
        pc += 2;
        printf("\n");
    }
    return 0;
}
