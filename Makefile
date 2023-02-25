out/bish: out/ls out/date out/cat out/mkdir out/rm src/bish.c
	@mkdir -p out
	@gcc -pthread -o out/bish src/bish.c
	@echo "\nComplied the bish shell (Bitch Interactive SHell)\n"
	@echo "To get the help regarding the commands of the shell refer to README.md\n"
	@echo "Run ./out/bish to start the shell"
	@echo "Or use 'make run' to run the shell\n"

out/ls: src/ls.c
	@mkdir -p out
	@gcc -o out/ls src/ls.c
	@echo "Compiled ls"


out/date: src/date.c
	@mkdir -p out
	@gcc -o out/date src/date.c
	@echo "Compiled date"

out/cat: src/cat.c
	@mkdir -p out
	@gcc -o out/cat src/cat.c
	@echo "Compiled cat"

out/mkdir: src/mkdir.c
	@mkdir -p out
	@gcc -o out/mkdir src/mkdir.c
	@echo "Compiled mkdir"

out/rm: src/rm.c
	@mkdir -p out
	@gcc -o out/rm src/rm.c
	@echo "Compiled rm"
	

clean:
	@rm -rf out
	@echo "Cleaned the executables"

run: out/bish
	@./out/bish
	
