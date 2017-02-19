CC	= gcc
OPT	= -Wall -g
COMP = $(CC) $(OPT) 

FILES	= chip8emu.c font.c
OUT_DIR	= out


build: $(FILES)
	$(COMP) -o $(OUT_DIR)/chip8 $(FILES)

dis: chip8dis.c
	$(COMP) -o $(OUT_DIR)/chip8dis chip8dis.c

window: window.c
	$(COMP) -o $(OUT_DIR)/window -L/usr/lib -lGL -lglut -lGLEW window.c

window2: window2.c
	$(COMP) -o $(OUT_DIR)/window2 -L/usr/lib -lGL -lglut -lGLEW window2.c


clean:
	rm -f $(OUT_DIR)/*
