# Compiler and flags
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Iheader_files

# Executable target
assembler: assembler.o pre_proc.o macros.o first_pass.o second_pass.o labels.o analyzer.o utility.o machine_code.o errors.o
	$(CC) $(CFLAGS) assembler.o pre_proc.o macros.o first_pass.o second_pass.o labels.o analyzer.o utility.o machine_code.o errors.o -o assembler

# Object file rules
assembler.o: source_files/assembler.c header_files/errors.h header_files/analyzer.h header_files/utility.h header_files/macros.h header_files/labels.h header_files/machine_code.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/assembler.c -o assembler.o

pre_proc.o: source_files/pre_proc.c header_files/pre_proc.h header_files/errors.h header_files/analyzer.h header_files/utility.h header_files/macros.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/pre_proc.c -o pre_proc.o

macros.o: source_files/macros.c header_files/macros.h header_files/errors.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/macros.c -o macros.o

first_pass.o: source_files/first_pass.c header_files/first_pass.h header_files/analyzer.h header_files/errors.h header_files/macros.h header_files/labels.h header_files/utility.h header_files/second_pass.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/first_pass.c -o first_pass.o

second_pass.o: source_files/second_pass.c header_files/second_pass.h header_files/errors.h header_files/analyzer.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/second_pass.c -o second_pass.o

labels.o: source_files/labels.c header_files/labels.h header_files/errors.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/labels.c -o labels.o

analyzer.o: source_files/analyzer.c header_files/analyzer.h header_files/errors.h header_files/utility.h header_files/macros.h header_files/labels.h header_files/machine_code.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/analyzer.c -o analyzer.o

utility.o: source_files/utility.c header_files/utility.h header_files/errors.h header_files/macros.h header_files/labels.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/utility.c -o utility.o

machine_code.o: source_files/machine_code.c header_files/machine_code.h header_files/errors.h header_files/analyzer.h header_files/labels.h header_files/macros.h header_files/utility.h header_files/definitions.h
	$(CC) $(CFLAGS) -c source_files/machine_code.c -o machine_code.o

errors.o: source_files/errors.c header_files/errors.h
	$(CC) $(CFLAGS) -c source_files/errors.c -o errors.o

# Clean up object files and the executable
clean:
	rm -f *.o assembler

