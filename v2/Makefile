CC = gcc
PTHREADLIB = -lpthread
OPENMPLIB = -fopenmp

all: asg3-pthread asg3-openmp

asg3-pthread: asg3-pthread.c util.c
	$(CC) -o $@ $^ $(PTHREADLIB) -O3

asg3-openmp: asg3-openmp.c util.c
	$(CC) -o $@ $^ $(OPENMPLIB) -O3

clean:
	rm asg3-pthread asg3-openmp
