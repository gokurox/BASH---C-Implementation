cc = gcc

srcPath = ./src
binaryPath = ./bin
execPath = ./exec

ccOutput = GShell.out

executeGShell: link $(execPath)/$(ccOutput)
	./$(execPath)/$(ccOutput)

link: compile
	$(cc) -o $(execPath)/$(ccOutput) $(binaryPath)/*.o

compile: Directories $(srcPath)/Codes/*.c
	$(cc) -c $(srcPath)/Codes/*.c
	mv *.o ./bin/

Directories: $(srcPath) $(binaryPath) $(execPath)

$(binaryPath):
	mkdir $(binaryPath)

$(execPath):
	mkdir $(execPath)

clean:
	rm -rf $(binaryPath) $(execPath)