CC := gcc
CFLAGS := -std=c11 -Wall -Werror -I./inc/

O := ./out/
C := ./src/
H := ./inc/

COMMON_H := $(H)io.h $(H)types.h

app.exe : $(O)main.o $(O)io.o $(O)graph_functions.o
	$(CC) $(CFLAGS) -o $@ $^
 
$(O)main.o : $(C)main.c $(COMMON_H) $(H)graph_functions.h
	$(CC) $(CFLAGS) -c $< -o $@

$(O)io.o : $(C)io.c $(COMMON_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(O)graph_functions.o : $(C)graph_functions.c $(COMMON_H) $(H)graph_functions.h
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY : clean img

clean :
	rm -f ./out/*.o *.exe

img :
	dot -Tpng graph.dot -o graph.png
