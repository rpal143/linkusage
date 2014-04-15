linkusage: linkusage.c
	gcc -o linkusage linkusage.c -Os -Werror -Wall

clean:
	rm linkusage

