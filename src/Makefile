
SUPPORT	=  utils.o tabset.o vec.o
CFLAGS	+= -I. -g -Delt_t=uint8_t
PROGS	+= entab detab

ALL:	$(PROGS)

entab:	entab.o $(SUPPORT)
detab:	detab.o  $(SUPPORT)


clean:
	rm -f *.o a.out

distclean: clean
	rm -f $(PROGS)

entab.o: entab.c errors.h utils.h tabset.h vec.h
vec.o: vec.c errors.h policy_size.h new.h vec.h
tabset.o: tabset.c utils.h errors.h new.h vec.h tabset.h
utils.o: utils.c errors.h utils.h
detab.o: detab.c errors.h new.h vec.h utils.h tabset.h
