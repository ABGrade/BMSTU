CC := gcc
CFLAGS := -std=c11 -D_POSIX_C_SOURCE=199309L -Wall -I./inc/

O := ./out/
C := ./src/
H := ./inc/

COMMON_H := $(H)io.h $(H)types.h $(H)hash_types.h
TREE_H := $(H)tree_functions.h $(H)tree_complex_functions.h 

TREE_O := $(O)tree_functions.o $(O)tree_complex_functions.o

app.exe : $(O)main.o $(O)io.o $(TREE_O) $(O)hash_functions.o $(O)common.o $(O)measure.o
	$(CC) $(CFLAGS) -o $@ $^
 
$(O)main.o : $(C)main.c $(COMMON_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(O)io.o : $(C)io.c $(COMMON_H) $(TREE_H) $(H)common.h $(H)measure.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(O)tree_functions.o : $(C)tree_functions.c $(COMMON_H) $(TREE_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(O)tree_complex_functions.o : $(C)tree_complex_functions.c $(COMMON_H) $(TREE_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(O)hash_functions.o : $(C)hash_functions.c $(COMMON_H) $(H)hash_functions.h
	$(CC) $(CFLAGS) -c $< -o $@

$(O)common.o : $(C)common.c $(COMMON_H) $(TREE_H) $(H)common.h $(H)hash_functions.h
	$(CC) $(CFLAGS) -c $< -o $@

$(O)measure.o : $(C)measure.c $(COMMON_H) $(TREE_H) $(H)hash_functions.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean img

clean :
	rm -f ./out/*.o *.exe

img :
	dot -Tpng tree.dot -o tree.png
