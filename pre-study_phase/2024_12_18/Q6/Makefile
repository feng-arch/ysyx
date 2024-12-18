CFALGS=-wall -g
all: ex6
ex6: ex6.o
	cc ex6.o -o ex6
%.o: %.c
	cc -c $< -o $@
clean:
	rm -rf *.o ex6