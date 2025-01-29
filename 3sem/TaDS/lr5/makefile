CFLAGS := -std=c99 -Wall -Werror -I./inc/

app.exe : 
	gcc $(CFLAGS) *.c -o app.exe
.PHONY : clean unit

clean :
	rm -f ./out/*.o *.exe

unit :
	./unit_tests.exe