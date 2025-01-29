CFLAGS := -std=c99 -Wall -I./inc/

app.exe : 
	gcc $(CFLAGS) *.c -o app.exe
.PHONY : clean 

clean :
	rm -f ./out/*.o *.exe

img :
	dot -Tpng tree.dot -o tree.png
