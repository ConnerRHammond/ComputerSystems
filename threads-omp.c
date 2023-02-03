#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[]) {
	int i;
	long myid;
	
#pragma omp parallel private(i,myid)
	{
		myid = omp_get_thread_num();
#pragma omp for
		for (i = 0; i < 10; i++) {
			sleep(1);
			printf("Hello %d from thread %d\n", i, myid);
		}
	}
}
