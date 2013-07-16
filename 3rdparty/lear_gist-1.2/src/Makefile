# Lear's GIST implementation, version 1.1, (c) INRIA 2009, Licence: PSFL


all: compute_gist

#WFFTINC=-I/usr/local/include
#WFFTLIB=-L/usr/local/lib64
WFFTINC=-I.
WFFTLIB=-L.


# gist.c: ../../descriptors/gist.c 
#	cp $< $@


gist.o: gist.c gist.h 
	gcc -c -Wall -g $< $(WFFTINC) -DUSE_GIST -DSTANDALONE_GIST

#standalone_image.o: standalone_image.c gist.h
#	gcc -c -Wall -g $< 

compute_gist: compute_gist.c gist.o
	gcc -Wall -g -o $@ $^ $(WFFTLIB) -lfftw3f -lm



clean: 
	rm -f *.o compute_gist

