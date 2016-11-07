CC = gcc
PTHREADLIB = -lpthread
OPENMPLIB = -fopenmp

all: asg3-pthread asg3-openmp

asg3-pthread: asg3-pthread.c
	$(CC) -o $@ $^ $(PTHREADLIB)

asg3-openmp: asg3-openmp.c
	$(CC) -o $@ $^ $(OPENMPLIB)

clean:
	rm asg3-pthread asg3-openmp
