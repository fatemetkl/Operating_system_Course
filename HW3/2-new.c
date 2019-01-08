
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int size, num_threads;
int *buffer;
int *result;
int fill=0;
int count=0;
int use=0;
pthread_cond_t empty, fill;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void put(int *value) {
buffer[fill] = *value;
fill = (fill + 1) % size;
count++;
}

int get() {
int *tmp = buffer[use];
use = (use + 1) % size;
count--;
return *tmp;
}




double **matrix1, **matrix2, **matrix3;


double ** allocate_matrix( int size )
{
    double * vals = (double *) malloc( size * size * sizeof(double) );

    double ** ptrs = (double **) malloc( size * sizeof(double*) );

    int i;
    for (i = 0; i < size; ++i) {
        ptrs[ i ] = &vals[ i * size ];
    }

    return ptrs;
}

void init_matrix( double **matrix, int size )
{

    int i, o;

    srand(time(NULL));
    for(o = 0; o<size; o++)
        for(i = 0; i<size; i++)
            matrix[o][i] = (rand())%100;

}

void print_matrix( double **matrix, int size )
{
    int i, j;

    for (i = 0; i < size; ++i) {
        for (j = 0; j < size-1; ++j) {
            printf( "%lf, ", matrix[ i ][ j ] );
        }
        printf( "%lf", matrix[ i ][ j ] );
        putchar( '\n' );
    }
}
//consumer
void *consumer(void *arg) {
    result = (double *) malloc( size*size * sizeof(double) );
    for(int i=0 ;i<size;i++){
Pthread_mutex_lock(&mutex);
while (count == 0)
Pthread_cond_wait(&fill, &mutex);
int *tmp = get();
int sum=0;
for(int j=0;j<size;j++){
    sum+=tmp[j];
}
printf("%d",sum);
Pthread_cond_signal(&empty);
Pthread_mutex_unlock(&mutex);

}


void * multi( void *arg )
{
    pthread_mutex_lock(&mutex);
    int i, j, k, tid, portion_size, row_start, row_end;
    double sum;
    int tmp[size];
    tid = *(int *)(arg);
    portion_size = size / num_threads;
    row_start = tid * portion_size;
    row_end = (tid+1) * portion_size;

    for (i = row_start; i < row_end; ++i) {
        for (j = 0; j < size; ++j) {
            sum = 0;
            for (k = 0; k < size; ++k) {
                sum += matrix1[ i ][ k ] * matrix2[ k ][ j ];
            }
            matrix3[ i ][ j ] = sum;
            tmp[j]=sum;
        }
    }
    while (count == size)
     Pthread_cond_wait(&empty, &mutex);
 put(*tmp);
    pthread_mutex_unlock(&mutex);
}

int main( int argc, char *argv[] )
{
    int i;
    double sum = 0;

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

    threads = (pthread_t *) malloc( (num_threads) * sizeof(pthread_t) );
    buffer = (double *) malloc( size * sizeof(double) );
    matrix1 = allocate_matrix( size );
    matrix2 = allocate_matrix( size );
    matrix3 = allocate_matrix( size );

    init_matrix( matrix1, size );
    init_matrix( matrix2, size );

    if ( size <= 10 ) {
        printf( "Matrix 1:\n" );
        print_matrix( matrix1, size );
        printf( "Matrix 2:\n" );
        print_matrix( matrix2, size );
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
        printf( "Matrix 3:\n" );
        print_matrix( matrix3, size );
    }


    return 0;
}