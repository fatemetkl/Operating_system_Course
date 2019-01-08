
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int size, num_threads;
double *result;
int *buffer;
int head=0;
int count=0;
int use=0;
pthread_cond_t empty=PTHREAD_COND_INITIALIZER;
pthread_cond_t fill=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void put(int value) {
    
buffer[head] = value;
head = (head + 1) % size;
count++;

}

int get() {
int tmp = buffer[use];
use = (use + 1) % size;
count--;
return tmp;
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
            matrix[o][i] = (rand())%3;

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
    int m=0;
    for(int i=0 ;i<size;i++){
pthread_mutex_lock(&mutex);
while (count == 0)
pthread_cond_wait(&fill, &mutex);
int tmp =get();
int sum=0;
for(int j=0;j<size;j++){
   sum+= matrix3[tmp][j];
}
 printf("'%s''%d'","here",sum);
result[m]=sum;
m++;
pthread_cond_signal(&empty);
pthread_mutex_unlock(&mutex);
}
}

void * multi( void *arg ){
    pthread_mutex_lock(&mutex);
    int i, j, k, tid, portion_size, row_start, row_end;
    double sum;
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
           
        }
    }
   while (count == size)
pthread_cond_wait(&empty, &mutex);
    put(tid);   
    pthread_cond_signal(&fill);
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
    buffer=(int *)malloc(size* sizeof(int *));
    matrix1 = allocate_matrix( size );
    matrix2 = allocate_matrix( size );
    matrix3 = allocate_matrix( size );
  result = (double *) malloc( size * sizeof(double) );
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
 pthread_t aggre;  
    pthread_create(&aggre, NULL, consumer, NULL);

    for ( i = 0; i < num_threads; ++i ) {
        pthread_join( threads[i], NULL );
    }
     pthread_join(aggre, NULL );
printf("'%s'\n","final vector");
for(int k=0; k<size;k++){
printf("'%lf'\n",result[k]);
}
printf("'%s'\n","final matrix");

    if ( size <= 10 ) {
        printf( "Matrix 3:\n" );
        print_matrix( matrix3, size );
    }


    return 0;
}