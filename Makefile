linkusage: linkusage.c
	gcc -o linkusage linkusage.c -Os -Werror -Wall

install: linkusage
	cp linkusage /usr/bin/

clean:
	rm -rf linkusage

