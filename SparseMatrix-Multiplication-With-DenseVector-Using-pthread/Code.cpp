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


// Algorith used --->  I divided the problem into subproblems and assigned each thread to solve a part of the problem. Each part calculates the result for each numberOfOperations = (n/noOfThreads) or (n/noOfThreads)+1;

int noOfThreads = 4; // number of threads
int threadNumber = 0; // thread number/part
int noOfOperations, noOfRows, noOfColumns, NNZ;
vector<vector<int>> A;
vector<int> B,C1,C2;

void multiplyMatrices(){
	for(int i=0;i<noOfRows;++i){
		int result = 0;
		for(int j=0;j<noOfColumns;++j){
				result+= A[i][j]*B[j];				
		}
		C1.push_back(result);
	}
}

// function for each thread
void *func(void *vargp)
{
    int currentThread = threadNumber; // thread id
    threadNumber++;
    
    // calculate result for each subtask :
    
    // If it is the last thread , calculate all the remaining result rows
    if(threadNumber == noOfThreads){
    	for(int i=(currentThread*noOfOperations);i<noOfRows;++i){
    		for(int j=0;j<noOfColumns;++j)
    			C2[i] = C2[i] + A[i][j]*B[j];	
    	}
    }
    // Else calculate no0fOperations results and store them
    else{
    	for(int i=(currentThread*noOfOperations);i<(currentThread+1)*noOfOperations;++i){
    		for(int j=0;j<noOfColumns;j++)
    			C2[i] = C2[i] + A[i][j]*B[j];	
    	}
    }
    
    return NULL;
}
 
int main()
{
    ifstream F1("inputfile.mtx");

    F1>>noOfRows>>noOfColumns>>NNZ;
  
    A.resize(noOfRows,vector<int>(noOfColumns,0));
    C2.resize(noOfRows, 0);
    for(int i=0;i<NNZ;i++){
        int q1,q2;
        F1>>q1>>q2;
        q1--;
        q2--;
        A[q1][q2] = 1;
    }
    
    
    cout<<"Please enter the number of threads\n";
    cin>>noOfThreads;
    
    ifstream F2("vector.txt");
    vector<string> currentRow;
    string line,temp,word,s;
    
    while(F2>>temp){
      string t2 = temp.substr(3,4);
      B.push_back(stoi(t2));
      getline(F2, line);
      stringstream s(line);
      while (getline(s, word, ',')) {
            currentRow.push_back(word);
            if(word[0]==' ')
            	B.push_back(stoi(word.substr(1,word.size()-1)));
            else
            	B.push_back(stoi(word));
      }        
   }
   
    //Displaying the matix statistics and system threads
    cout<<"#Rows --> "<<noOfRows<<"\n";
    cout<<"#Cols : "<<noOfColumns<<"\n";
    cout<<"#Non-Zeroes --> "<<NNZ<<"\n";
    cout<<"#threads --> "<<noOfThreads<<"\n";
    //for(auto i:dense_vect)cout<<i<<endl;

    //to calculate time
    time_t startTime, endTime;

    time(&startTime);

    multiplyMatrices();
    
    
    pthread_t threads[noOfThreads];
    
    if(noOfRows%noOfThreads == 0)
    	noOfOperations = noOfRows/noOfThreads;
    else
    	noOfOperations = (noOfRows/noOfThreads)+1;
    
    // creating threads
    for (int i = 0; i < noOfThreads; ++i)
        pthread_create(&threads[i], NULL, func, NULL);
    
    // joining threads
    for (int i = 0; i < noOfThreads; ++i)
        pthread_join(threads[i], NULL);
 
    
    time(&endTime);
    double timer = double(endTime - startTime);
    cout << "Time taken by the code segment is : " << fixed<< timer << setprecision(5);
    cout << " seconds " << endl;
    
    pthread_exit(NULL);
    return 0;
    
    // I noticed that the time taken is almost constant for this sparse matrix and dense vector multiplication. However, with increased size of the sparse matrix, the time taken becomes smaller with increasing number of threads. 
}
