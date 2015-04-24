OBJ	:=	../myserver.bin
SOURS	:=	Myserver.c Main.c

MACRO	=	DEBUGALL
CFLAGS	+=	-g -w -lpthread -D$(MACRO)
CC	=	gcc

$(OBJ):$(SOURS)
	$(CC) $(CFLAGS) $^ -o $@
	
.PHONY:
	rebuild \
	clean

rebuild:
	make clean
	make $(OBJ)
	
clean:
	rm -f $(OBJ)
