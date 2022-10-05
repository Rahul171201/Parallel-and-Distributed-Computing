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


// Algorith used --->  I firstly generated a random binary matrix and converted it into an adjacency list. I then used Rem's algorithm to calculate the
// number of connected components in the graph. I assigned each thread (n*n)/no_of_threads nodes to calculate the union find. After each
// thread calculates its piece of work, we finally get the total n umber opf connected components as the number of elements in our map M.

map<int, set<int>> M;
int no_of_threads = 4; // number of threads
int part = 0; // thread number/part
int n; // number of rows and columns in the binary matrix

void generateRandomBinaryMatrix(vector<vector<int>> &binary_matrix){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            binary_matrix[i][j] = (rand() %(1 - 0 + 1)) + 0;
        }
    }
}

int findSet(int node, vector<int> &parent){
    int ans = node;
    while(parent[node]!=-1){
        node = parent[node];
        ans = node;
    }
    return ans;
}

void makeUnion(vector<vector<int>> &adjacencyMatrix, vector<int> &parent ,int node){
    if(parent[node]==-1){
        M[node].insert(node);
    }
    cout<<node<<"\n";
    for(int i=0;i<adjacencyMatrix[node].size();i++){
        if(parent[node] == parent[adjacencyMatrix[node][i]] && parent[node]!=-1)
            continue;
        if(parent[node]==-1 && parent[adjacencyMatrix[node][i]]==-1){
            if(node > adjacencyMatrix[node][i]){
                M[node].insert(adjacencyMatrix[node][i]);
                parent[adjacencyMatrix[node][i]] = node;
            }
            else{
                M[adjacencyMatrix[node][i]].insert(adjacencyMatrix[node][i]);
                M[adjacencyMatrix[node][i]].insert(node);
                M[node].clear();
                parent[node] = adjacencyMatrix[node][i];
            }
        }
        else if(parent[node]==-1 && parent[adjacencyMatrix[node][i]]!=-1){
            int currentMainNode = findSet(adjacencyMatrix[node][i], parent);
            if(node > currentMainNode){
                M[node].insert(currentMainNode);
                M[currentMainNode].clear();
                parent[currentMainNode] = node;
            }
            else{
                M[currentMainNode].insert(node);
                M[node].clear();
                parent[node] = currentMainNode;
            }
        }
        else if(parent[node]!=-1 && parent[adjacencyMatrix[node][i]]==-1){
            int currentMainNode = findSet(node, parent);
            if(currentMainNode > adjacencyMatrix[node][i]){
                M[currentMainNode].insert(adjacencyMatrix[node][i]);
                M[adjacencyMatrix[node][i]].clear();
                parent[adjacencyMatrix[node][i]] = currentMainNode;
            }
            else{
                M[adjacencyMatrix[node][i]].insert(currentMainNode);
                M[currentMainNode].clear();
                parent[currentMainNode] = adjacencyMatrix[node][i];
            }
        }
        else{
            int myNode = findSet(node, parent);
            int neighbourNode = findSet(adjacencyMatrix[node][i], parent);
            if(myNode > neighbourNode){
                M[myNode].insert(neighbourNode);
                M[neighbourNode].clear();
                parent[neighbourNode] = myNode;
            }
            else{
                M[neighbourNode].insert(myNode);
                M[myNode].clear();
                parent[myNode] = neighbourNode;
            }
        }
    }
}

// function for each thread
// void *threadFunction(void *vargp)
// {
//     int myid = part; // thread id
//     part++;
    
//     // calculate each part
//     for(int i=part*((n*n)/no_of_threads);i<(part+1)*((n*n)/no_of_threads);i++){
//         makeUnion(adjacencyMatrix, parent, i);
//     }
//     return NULL;
// }
 
int main()
{
    // to calculate time
    time_t startTime, endTime;

    time(&startTime);

    
    pthread_t threads[no_of_threads];
    
    cout<<"Enter the value of n\n";
    cin>>n;

    // generate randome binary matrix of size n x n
    vector<vector<int>> binary_matrix(n, vector<int> (n,0));
    
    generateRandomBinaryMatrix(binary_matrix);

    cout<<"The binary matrix is :\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<binary_matrix[i][j]<<" ";
        }
        cout<<"\n";
    }

    vector<vector<int>> adjacencyMatrix(n*n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(binary_matrix[i][j]==1){
                if(i-1>=0 && binary_matrix[i-1][j]==1)
                    adjacencyMatrix[i*n+j].push_back((i-1)*n+j);
                if(i+1<n && binary_matrix[i+1][j]==1)
                    adjacencyMatrix[i*n+j].push_back((i+1)*n+j);
                if(j-1>=0 && binary_matrix[i][j-1]==1)
                    adjacencyMatrix[i*n+j].push_back(i*n+(j-1));
                if(j+1<n && binary_matrix[i][j+1]==1)
                    adjacencyMatrix[i*n+j].push_back(i*n+(j+1));
            }
        }
    }
    vector<int> parent(n*n, -1);

    for(int i=0;i<n*n;i++){
        cout<<"The neighbours of "<<i<<" are : ";
        for(int neighbour =0; neighbour<adjacencyMatrix[i].size(); neighbour++){
            cout<<adjacencyMatrix[i][neighbour]<<" ";
        }
        cout<<"\n";
    }

    for(int i=0;i<n*n;i++){
        makeUnion(adjacencyMatrix, parent, i);
    }    

    int connectedComponents = 0;
    for(auto it : M){
        if(!M[it.first].empty()){
            connectedComponents++;
        }
    }

    cout<<"The number of connected components is "<<connectedComponents<<"\n";
    
    // creating threads
    // for (int i = 0; i < no_of_threads; i++)
    //     pthread_create(&threads[i], NULL, threadFunction, NULL);
    
    
    // // joining threads
    // for (int i = 0; i < no_of_threads; i++)
    //     pthread_join(threads[i], NULL);
        
 
   
    
    time(&endTime);
    double timer = double(endTime - startTime);
    cout << "Time taken by the code segment is : " << fixed<< timer << setprecision(5);
    cout << " seconds " << endl;
    
    pthread_exit(NULL);
    return 0;
}
