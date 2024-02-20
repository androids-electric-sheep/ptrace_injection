all:
	clang-format -i *.c
	gcc ptrace_injection.c -o ptrace_injection.bin
clean:
	rm -vf *.bin
build_test:
	gcc test.c -o test.bin
