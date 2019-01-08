
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int size, num_threads;
double *vector1, *vector2, *vector3;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void init_matrix( double *vector, int size )
{

    int i;
    srand(time(NULL));

        for(i = 0; i<size; i++)
            vector[i] =(rand());

}

void print_matrix( double *vector, int size )
{
    int  j;


        for (j = 0; j < size-1; ++j) {
            printf("%lf, ", vector[j]);
        }
        putchar( '\n' );

}


void * multi( void *arg )
{
    pthread_mutex_lock(&mutex);
    int i, tid, portion_size,start,end;

    tid = *(int *)(arg);
    portion_size = size / num_threads;
    start = tid * portion_size;
    end = (tid+1) * portion_size;

    for (i = start; i < end; ++i) {

      vector3[i]= vector1[i]* vector2[i];
    }
    pthread_mutex_unlock(&mutex);
}

int main( int argc, char *argv[] )
{
    int i;

    pthread_t * threads;

    if (argc < 1)
    {
        printf("%d",argc);
    }
    printf("Enter vector length: ");

    scanf("%d", &size);
    printf("Enter number of threads: ");

    scanf("%d", &num_threads);


    if ( size % num_threads != 0 ) {
        fprintf( stderr, "size %d must be a multiple of num of threads %d\n",
                 size, num_threads );
        return -1;
    }

    threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );

     vector1 = (double *) malloc( size * sizeof(double) );
    vector2 = (double *) malloc( size * sizeof(double) );
    vector3 = (double *) malloc( size * sizeof(double) );
    init_matrix(vector1, size );
    init_matrix( vector2, size );

    if ( size <= 10 ) {
        printf( "vector 1:\n" );
        print_matrix( vector1, size );
        printf( "vector 2:\n" );
        print_matrix( vector2, size );
    }


    for ( i = 0; i < num_threads; ++i ) {
        int *tid;
        tid = (int *) malloc( sizeof(int) );
        *tid = i;
        pthread_create( &threads[i], NULL, multi, (void *)tid );
    }

    for ( i = 0; i < num_threads; ++i ) {
        pthread_join( threads[i], NULL );
    }


    if ( size <= 10 ) {
        printf( "vector 3:\n" );
        print_matrix( vector3, size );
    }


    return 0;
}
 