# Compiler and flags
CC = gcc
CFLAGS = -ansi -pedantic -Wall -IHeader\ Files

# Executable target
assembler: assembler.o pre_proc.o macros.o first_pass.o second_pass.o labels.o analyzer.o utility.o machine_code.o errors.o
	$(CC) $(CFLAGS) assembler.o pre_proc.o macros.o first_pass.o second_pass.o labels.o analyzer.o utility.o machine_code.o errors.o -o assembler

# Object file rules
assembler.o: Source\ Files/assembler.c Header\ Files/errors.h Header\ Files/analyzer.h Header\ Files/utility.h Header\ Files/macros.h Header\ Files/labels.h Header\ Files/machine_code.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/assembler.c -o assembler.o

pre_proc.o: Source\ Files/pre_proc.c Header\ Files/pre_proc.h Header\ Files/errors.h Header\ Files/analyzer.h Header\ Files/utility.h Header\ Files/macros.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/pre_proc.c -o pre_proc.o

macros.o: Source\ Files/macros.c Header\ Files/macros.h Header\ Files/errors.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/macros.c -o macros.o

first_pass.o: Source\ Files/first_pass.c Header\ Files/first_pass.h Header\ Files/analyzer.h Header\ Files/errors.h Header\ Files/macros.h Header\ Files/labels.h Header\ Files/utility.h Header\ Files/second_pass.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/first_pass.c -o first_pass.o

second_pass.o: Source\ Files/second_pass.c Header\ Files/second_pass.h Header\ Files/errors.h Header\ Files/analyzer.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/second_pass.c -o second_pass.o

labels.o: Source\ Files/labels.c Header\ Files/labels.h Header\ Files/errors.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/labels.c -o labels.o

analyzer.o: Source\ Files/analyzer.c Header\ Files/analyzer.h Header\ Files/errors.h Header\ Files/utility.h Header\ Files/macros.h Header\ Files/labels.h Header\ Files/machine_code.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/analyzer.c -o analyzer.o

utility.o: Source\ Files/utility.c Header\ Files/utility.h Header\ Files/errors.h Header\ Files/macros.h Header\ Files/labels.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/utility.c -o utility.o

machine_code.o: Source\ Files/machine_code.c Header\ Files/machine_code.h Header\ Files/errors.h Header\ Files/analyzer.h Header\ Files/labels.h Header\ Files/macros.h Header\ Files/utility.h Header\ Files/definitions.h
	$(CC) $(CFLAGS) -c Source\ Files/machine_code.c -o machine_code.o

errors.o: Source\ Files/errors.c Header\ Files/errors.h
	$(CC) $(CFLAGS) -c Source\ Files/errors.c -o errors.o

# Clean up object files and the executable
clean:
	rm -f *.o assembler
