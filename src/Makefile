
CFLAGS	+= -Wall -g -I. -Delt_t="uint8_t"

OBJ	= detab.o vec.o
PROG	= detab

$(PROG):	$(OBJ)


clean:
	rm -f *.o a.out 

distclean:	clean
	rm	-f $(PROG)
	
detab.o: detab.c errors.h new.h vec.h
vec.o: vec.c errors.h policy_size.h new.h vec.h
