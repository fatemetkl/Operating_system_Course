#HW3
#Q1
1. You have a serial version of vector dot product computation. Change this
program to do the computation using multiple threads.
Your program should input the length of vectors and the number of threads
from the user (t), then initialize the two vectors with random values, divide
the vectors to (t) parts. Each thread computes its own part and updates the
final value. The main thread prints the final dot product.
For Simplicity, assume that the vector length is divisible to t.

2. Now write a multithreaded matrix multiplication code which computes the
sum of the rows of result matrix as a vector: (see the figure and example)


###more about questions in pdf file






## important note about question2
the final code for second question is "2-final.c"                      
  it has consumer and multiply function as producer :                                          
    ##  i used signals to wake up consumer                      
    ## there is a count flag 
    ## it has a buffer[size] wich keeps track of rows of final matrix                              
    ## the consumer function adds it elements into sum then prints them                                       
    ## its outputs are final vector and final matrix
    
## locks for 2-final    
    both producer (multi) and consumer has mutex and also they wait if the buffer is empty or fill
    signals are used to wake them up
    
**       


## compile and run:
   
      for compiling vector multiplication :                                                
                ''' gcc 1.c -pthread'''
       for compiling matrix multiplication :                                                                     
                  '''  gcc 2-final.c -pthread'''
****by entering the matrixes sizes the program creates a random initialized matrix(vector) with given size
****then it will creat t(given number of threads) thrads that do the multiplications

 I used pthread_mutex_lock() 
because we had a critical rigion and it was the simplest and better one for threads
and there was no need of semaphores according to its definition and usage

**********
## definitions:
Semaphore: Use a semaphore when you (thread) want to sleep till some other thread tells you to wake up. Semaphore 'down' happens in one thread (producer) and semaphore 'up' (for same semaphore) happens in another thread (consumer)


Mutex: Use a mutex when you (thread) want to execute code that should not be executed by any other thread at the same time. Mutex 'down' happens in one thread and mutex 'up' must happen in the same thread later on.
.

Spinlock: Use a spinlock when you really want to use a mutex but your thread is not allowed to sleep.



 The mutex object referenced by mutex is locked by calling pthread_mutex_lock(). If the mutex is already locked, the calling thread blocks until the mutex becomes available. This operation returns with the mutex object referenced by mutex in the locked state with the calling thread as its owner.  interrupt handler, acquire a spinlock - insert node - r


