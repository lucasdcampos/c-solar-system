gcc -o solars -Iinclude/ *.c -Llib/ -lraylib -Wl,-rpath=lib/ -lm && ./solars
