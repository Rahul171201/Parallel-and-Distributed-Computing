#include<bits/stdc++.h>
#include<pthread.h>
#include<omp.h>

using namespace std;

// Name -> Rahul Roy
// Roll -> IIT2019194

// Compiler used --> G++
// command to compile -> g++ code.cpp -lpthread -fopenmp
// command to run -> ./a.out
 
// Model name ----> Intel(R) Core(TM) i3-7020U CPU @ 2.30GHz
// Number of CPU's ---> 4
// Number of cores per socket ---> 24
// Architecture  ------>  x86_64
// CPU op-mode(s) ------>  32-bit, 64-bit


// Algorith used --->  I divided the problem into subproblems and assigned each thread to solve a part of the problem.
// Each part calculates the result for each numberOfOperations = (noOfColumns/noOfThreads) or (noOfColumns/noOfThreads)+1;
// This is one of the most basic approaches towards parallel algorithms and this is just to demonstrate 
// the sparse matrix multiplication of a sparse matrix with a dense matrix using
// both sequential algorithm as well as pthread and OPENMP libraries.

int noOfThreads = 4; // number of threads
int threadNumber = 0; // thread number/part
int noOfOperations; // number of operations to be carried out by each thread
int noOfRows; // number of rows
int noOfColumns; // number of columns
int NNZ; // number of non-zero elements in the sparse matrix
vector<int> A_row, A_column; // sparse matrix row and column array respectively
vector<double> A_value; // sparse matrix value array
vector<vector<double>> B; // dense matrix
vector<vector<double>> C1,C2,C3;

// Function to multiply both matrices sequentially
void multiplyMatricesSequentially(){
	C1.resize(noOfRows, vector<double>(noOfColumns,0));
    for(int col=0; col<noOfColumns;col++){
        for(int k=0;k<A_row.size();k++){
            C1[A_row[k]][col]+= A_value[k]*B[A_column[k]][col];
        }
    } 

    cout<<"After sequential multiplication of sparse matrix A with dense matrix B, we get C1 :\n";
    for(int i=0;i<noOfRows;i++){
        for(int j=0;j<noOfColumns;j++){
            cout<<C1[i][j]<<" ";
        }
        cout<<"\n";
    }

    return;
}

// Function for each thread
void *threadFunction(void *vargp)
{
    int currentThread = threadNumber; // thread id
    threadNumber++;
    
    // calculate result for each subtask :
    
    // If it is the last thread , calculate all the remaining result rows
    if(threadNumber == noOfThreads){
    	for(int col=(currentThread*noOfOperations);col<noOfColumns;col++){
    		for(int k=0;k<A_row.size();k++)
    			C2[A_row[k]][col] = C2[A_row[k]][col] + A_value[k]*B[A_column[k]][col];	
    	}
    }
    // Else calculate no0fOperations results and store them
    else{
        for(int col=(currentThread*noOfOperations);col<(currentThread+1)*noOfOperations;col++){
    		for(int k=0;k<A_row.size();k++)
    			C2[A_row[k]][col] = C2[A_row[k]][col] + A_value[k]*B[A_column[k]][col];	
    	}
    }
    return NULL;
}
 
int main()
{
    // number of threads as input from the user
    cout<<"Enter the number of threads\n";
    cin>>noOfThreads;
    
   
    ifstream F1("inputfile.mtx"); // read inputfile.mtx into F1

    F1>>noOfRows>>noOfColumns>>NNZ;

    // resizing 2-D vectors
    B.resize(noOfRows, vector<double>(noOfColumns, 0));
    C2.resize(noOfRows, vector<double>(noOfColumns, 0));
    C3.resize(noOfRows, vector<double>(noOfColumns, 0));
  
    for(int i=0;i<NNZ;i++){
        int q1,q2;
        double val;
        F1>>q1>>q2>>val;
        A_row.push_back(q1-1);
        A_column.push_back(q2-1);
        A_value.push_back(val);
    }

    cout<<"#Rows = "<<noOfRows<<"\n";
    cout<<"#Cols = "<<noOfColumns<<"\n";
    cout<<"#Non-Zeroes = "<<NNZ<<"\n";
    cout<<"#threads = "<<noOfThreads<<"\n";

    cout<<"Matrix A :\n";
    cout<<"Row   Column   Value\n";
    for(int i=0;i<NNZ;i++){
        cout<<A_row[i]<<"      "<<A_column[i]<<"      "<<A_value[i]<<"\n";
    }
    
    
    
    ifstream F2("matrix.txt"); // reading matrix.txt into F2
    string temp;
    
    // store the values in B
    int count = 0;
    int currentRow = 0;
    while(F2>>temp){
      string t2;
      if(count!=0)
        t2 = temp.substr(1,temp.size()-1);
      else
        t2 = temp.substr(3, t2.size()-3);
        
      double tk = stof(t2);
      B[currentRow][count] = tk; 
      count++;
      if(count==138){
        currentRow++;
        count = 0;
      }  
   }
   

   cout<<"Matrix B :\n";
   for(int i=0;i<noOfRows;i++){
    for(int j=0;j<noOfColumns;j++){
        cout<<B[i][j]<<" ";
    }
    cout<<"\n";
   }

    // calling function to multiply matrices A and B sequentially
    multiplyMatricesSequentially();

    //to calculate time
    time_t startTime, endTime;

    time(&startTime);
    
    
    // declaring threads
    pthread_t threads[noOfThreads];
    
    // distributing operations between threads
    if(noOfColumns%noOfThreads == 0)
    	noOfOperations = noOfColumns/noOfThreads;
    else
    	noOfOperations = (noOfColumns/noOfThreads)+1;
    
    // creating threads
    for (int i = 0; i < noOfThreads; ++i)
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    
    // joining threads
    for (int i = 0; i < noOfThreads; ++i)
        pthread_join(threads[i], NULL);
 
    cout<<"After parallel multiplication of sparse matrix A with dense matrix B using pthread library, we get C2 :\n";
    for(int i=0;i<noOfRows;i++){
        for(int j=0;j<noOfColumns;j++){
            cout<<C2[i][j]<<" ";
        }
        cout<<"\n";
    }

    // to check if C1 and C2 are same or not
    bool isSame1 = true, isSame2 = true;;
    for(int i=0;i<noOfRows;i++){
        for(int j=0;j<noOfColumns;j++){
            if(C1[i][j] != C2[i][j]){
                isSame1 = false;
                break;
            }
        }
    }

    if(isSame1)
        cout<<"C1 and C2 are same\n";
    else
        cout<<"Not same\n";

    time(&endTime);
    double timer = double(endTime - startTime);
    // cout << "Time taken by the pthread code segment is : " << fixed<< timer << setprecision(5);
    // cout << " seconds " << endl;

    //to calculate time
    time(&startTime);

    // Parallel matrix multiplication using OPENMP
    #pragma omp parallel num_threads(noOfThreads)
    {
        // Getting thread number
        int threadId = omp_get_thread_num();
  
         // If it is the last thread , calculate all the remaining result rows
        if(threadId == noOfThreads-1){
    	    for(int col=(threadId*noOfOperations);col<noOfColumns;col++){
    		    for(int k=0;k<A_row.size();k++)
    			    C3[A_row[k]][col] = C3[A_row[k]][col] + A_value[k]*B[A_column[k]][col];	
    	    }
        }
        // Else calculate no0fOperations results and store them
        else{
            for(int col=(threadId*noOfOperations);col<(threadId+1)*noOfOperations;col++){
    		    for(int k=0;k<A_row.size();k++)
    			    C3[A_row[k]][col] = C3[A_row[k]][col] + A_value[k]*B[A_column[k]][col];	
    	    }
        }
    }

    cout<<"After parallel multiplication of sparse matrix A with dense matrix B using OPENMP library, we get C3 :\n";
    for(int i=0;i<noOfRows;i++){
        for(int j=0;j<noOfColumns;j++){
            cout<<C3[i][j]<<" ";
        }
        cout<<"\n";
    }
    

     for(int i=0;i<noOfRows;i++){
        for(int j=0;j<noOfColumns;j++){
            if(C2[i][j] != C3[i][j]){
                isSame2 = false;
                break;
            }
        }
    }

    if(isSame2)
        cout<<"C2 and C3 are same\n";
    else
        cout<<"Not same\n";

    
    time(&endTime);
    timer = double(endTime - startTime);
    // cout << "Time taken by the OMP Code segment is : " << fixed<< timer << setprecision(5);
    // cout << " seconds " << endl;
    
    pthread_exit(NULL);
    return 0;
    
    // I noticed that the time taken gradually decreases with increase
    // in number of threads. However, time taken increases with increase in number of threads after a threshold value. 
    // Through expriment, I found that the ideal number of threads = 4;
}
