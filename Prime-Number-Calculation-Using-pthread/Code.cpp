#include<bits/stdc++.h>
#include<pthread.h>

using namespace std;

// Author -> Rahul Roy

// Compiler used --> G++
// command to compile -> g++ q1.cpp -lpthread
// command to run -> ./a.out
 
// Model name ----> Intel(R) Core(TM) i3-7020U CPU @ 2.30GHz
// Number of CPU's ---> 4
// Number of cores per socket ---> 24
// Architecture  ------>  x86_64
// CPU op-mode(s) ------>  32-bit, 64-bit


// Algorith used --->  I divided the problem into subproblems and assigned each thread to solve a part of the problem. Each part calculates the number of primes between (2^N/n)*i to (2^N/n)*(i+1).
// The global variable named 'counter' keeps the track of the number of primes. I used the time library of c to calculate the execution time.



int counter; // to store the count of prime numbers between 1 to 2^N;
int no_of_threads = 4; // number of threads
int part = 0; // thread number/part
int N; // value of N in 2^N

// function to check if a number is prime
int checkPrimeNumber(int n)
{
    if (n <= 1)
        return 0; 
    for (int i = 2; i < n; ++i)
        if (n % i == 0)
            return 0;
 
    return 1;
}

// function for each thread
void *threadFunction(void *vargp)
{
    int myid = part; // thread id
    part++;
    
    // calculate each part
    for(int i = ((pow(2,N))/no_of_threads)*myid; i<((pow(2,N))/no_of_threads)*(myid+1) ; i++){
    	if(checkPrimeNumber(i)){
    		counter++;
    	};
    }
    return NULL;
}
 
int main()
{
    // to calculate time
    time_t startTime, endTime;

    time(&startTime);

    
    pthread_t threads[no_of_threads];
    
    cout<<"Enter the value of N\n";
    cin>>N;
    
    counter=0;
    
    // creating threads
    for (int i = 0; i < no_of_threads; i++)
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    
    
    // joining threads
    for (int i = 0; i < no_of_threads; i++)
        pthread_join(threads[i], NULL);
        
    cout<<"The number of prime numbers between 1 and 2^"<<N<<" is = "<<counter<<"\n";
 
   
    
    time(&endTime);
    double timer = double(endTime - startTime);
    cout << "Time taken by the code segment is : " << fixed<< timer << setprecision(5);
    cout << " seconds " << endl;
    
    pthread_exit(NULL);
    return 0;
}
