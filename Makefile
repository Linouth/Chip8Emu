CC	= gcc
OPT	= -Wall -g
COMP = $(CC) $(OPT) 

FILES	= emulator.c
OUT_DIR	= out


build: $(FILES)
	$(COMP) -o $(OUT_DIR)/chip8 $(FILES)

dis: chip8dis.c
	$(COMP) -o $(OUT_DIR)/chip8dis chip8dis.c


clean:
	rm -f $(OUT_DIR)/*
