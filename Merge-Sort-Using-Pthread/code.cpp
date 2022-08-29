#include<bits/stdc++.h>
#include<pthread.h>

using namespace std;

// Name -> Rahul Roy
// Roll -> IIT2019194

// Compiler used --> G++
// command to compile -> g++ q1.cpp -lpthread
// command to run -> ./a.out
 
// Model name ----> Intel(R) Core(TM) i3-7020U CPU @ 2.30GHz
// Number of CPU's ---> 4
// Number of cores per socket ---> 24
// Architecture  ------>  x86_64
// CPU op-mode(s) ------>  32-bit, 64-bit


// Algorith used ---> Firstly in order to sort and merge the arrays into one, I declare a global
// array C which stores the merged result of both sorted arrays. Then I assign each thread to perform operation on (n+m)/k
// size partition in C, where n--> number of elements in A and m--> number of elements in B and k-->number of threads.
// Then I find a (i,j) for each partition such that max(A[i-1], B[j-1]) < min(A[i], B[j]) so as to sort without any dependency correction. 
// Finally after each thread performs its piece of task, I join all the threads.

int no_of_threads = 4; // number of threads
int part = 0; // thread number/part
vector<char> A, B, C;
vector<pair<int,int>> partition_points;
int m,n;
int partition_size, current_a=0, current_b=0, prev_a=0, prev_b=0;

// search for i,j such that max(A[i-1], B[j-1]) < min(A[i], B[j]);
bool searchPartition(){
    int temp1 = INT_MIN, temp2 = INT_MIN;
    if(current_a>prev_a)
        temp1 = A[current_a-1];
    if(current_b>prev_b)
        temp2 = B[current_b-1];
    if(max(A[current_a-1], B[current_b-1]) < min(A[current_a], B[current_b]))
        return true;
    else
        return false;
}

void mergePartition(int start_a, int start_b, int end_a,int end_b){
    int current = part*partition_size;
    for(int i=part*partition_size;i<(part+1)*partition_size;i++){
        if(start_a>= end_a || start_b>=end_b){
            break;
        }
        if(A[start_a] < B[start_b]){
            C[i] = A[start_a];
            start_a++;
        }
        else{
            C[i] = B[start_b];
            start_b++;
        }
        current = i+1;;
    }
    for(int i=start_a;i<end_a;i++){
        C[current] = A[start_a];
        current++;
    }
    for(int i=start_b;i<end_b;i++){
        C[current] = A[start_b];
        current++;
    }
}


// function for each thread
void *threadFunction(void *vargp)
{
    if(part==0)
        mergePartition(0, 0, partition_points[part].first, partition_points[part].second);
    else
        mergePartition(partition_points[part-1].first, partition_points[part-1].second, partition_points[part].first, partition_points[part].second);
    part++;
    return NULL;
}

 
int main()
{
    // to calculate time
    time_t startTime, endTime;

    time(&startTime);

    
    pthread_t threads[no_of_threads];
    
    cout<<"Enter the size of array A\n";
    cin>>m;
    A.resize(m,0);
    cout<<"Enter the elements of array A\n";
    for(int i=0;i<m;i++){
        cin>>A[i];
    }

    cout<<"Enter the size of array B\n";
    cin>>n;
    B.resize(n,0);
    cout<<"Enter the elements of array B\n";
    for(int i=0;i<n;i++){
        cin>>B[i];
    }

    C.resize(m+n,0);
    partition_size = (n+m)/no_of_threads;
    current_a = partition_size;
    
    for(int i=0;i<no_of_threads;i++){
        cout<<i<<"\n";
        if(i==no_of_threads-1){
            partition_points.push_back({m,n});
            break;
        }
        if(current_a>m){
            current_b = current_b + (current_a - m);
            current_a = m;
        }
        while (!searchPartition())
        {
            current_a--;
            current_b++;
        }
        prev_a = current_a;
        prev_b = current_b;
        partition_points.push_back({current_a, current_b});
        current_a = partition_size + current_a;
    }

    exit(0);

    for(int i=0;i<partition_points.size();i++){
        cout<<partition_points[i].first<<" "<<partition_points[i].second<<"\n";
    }
    
    // creating threads
    for (int i = 0; i < no_of_threads; i++)
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    
    
    // joining threads
    for (int i = 0; i < no_of_threads; i++)
        pthread_join(threads[i], NULL);
   
    
    time(&endTime);
    double timer = double(endTime - startTime);
    cout << "Time taken by the code segment is : " << fixed<< timer << setprecision(5);
    cout << " seconds " << endl;
    
    // pthread_exit(NULL);
    return 0;
}
