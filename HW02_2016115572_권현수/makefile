OBJECTS =  sum sub fact div

myshell : $(OBJECTS)
	gcc -o myshell myshell.c

.c.o:
	gcc -o $@ 

clean :
	rm -f $(OBJECTS)
	rm -f myshell
