/* 
  To Compile: gcc -O -Wall lab12.c -lpthread
  To Run: ./a.out 1000 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct my_struct{
	double *a; 
	double *partialsum;
	int N;
	int nthreads;
	int size;
	double sum;
	long count;
	pthread_t *tid;
} my_struct;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


void *compute(void *arg) {
    int myStart, myEnd, myN, i;
    long tid = (long)arg;

	my_struct *struct_ptr = (my_struct*) arg;
	
    // determine start and end of computation for the current thread
    myN = struct_ptr->N/struct_ptr->nthreads;
	
    myStart = tid*myN;
    myEnd = myStart + myN;
	
    if (tid == (struct_ptr->nthreads-1)) myEnd = struct_ptr->N;

    // compute partial sum
    double mysum = 0.0;
    for (i=myStart; i<myEnd; i++)
      mysum += struct_ptr->a[i];

    struct_ptr->partialsum[tid] = mysum;
    return (NULL);
}

int main(int argc, char **argv) {
    long i;
    pthread_t *tid;
	my_struct *info;
    double sum = 0.0;

    if (argc != 3) {
       printf("Usage: %s <# of elements> <# of threads>\n",argv[0]);
       exit(-1);
    }

    info->N = atoi(argv[1]); // no. of elements
    info->nthreads = atoi(argv[2]); // no. of threads

    // allocate vector and initialize
	info->tid = (pthread_t *)malloc(sizeof(pthread_t)*(info->nthreads));
    info->a = (double *)malloc(sizeof(double)*info->N); 
    info->partialsum = (double *)malloc(sizeof(double)*(info->nthreads)); 
	
    for (i=0; i<info->N; i++)
      info->a[i] = (double)(i + 1);

    // create threads
    for ( i = 0; i < info->nthreads; i++){
      pthread_create(&info->tid[i], NULL, compute, (void *)info);
	}
    
    // wait for them to complete
    for ( i = 0; i < info->nthreads; i++){
      pthread_join(info->tid[i], NULL);
	}

    for ( i = 0; i < info->nthreads; i++){
      info->sum += info->partialsum[i];
	}
    
    printf("The total is %g, it should be equal to %g\n", 
           info->sum, ((double)info->N*((info->N)+1))/2);
    
    free(info->tid);
    free(info->a);
    free(info->partialsum);

    return 0;
}


/* References: https://www.codegrepper.com/code-examples/c/How+to+pass+a+struct+value+to+a+pthread+in+c%3F */