#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int main()
{
	int n = 1000;

	double* array = new double[n];
	for(int i = 0; i < n; i++)
		array[i] = i;

	double sum=0;
	double parallel_sum = 0;
	double parallel_critical_sum = 0;
	double parallel_for_critical_sum = 0;
	double parallel_for_reductive_sum = 0;
	double lsum = 0;


	// Serialized sum
	for(int i = 0; i < n; i++)
		sum += array[i];
	
	// To set the number of threads
	omp_set_num_threads(4);

	// Parallel Directive
	#pragma omp parallel num_threads(4)
	{
		int tid=omp_get_thread_num();
		cout<<"Hello from thread : "<< tid<<"\n";
		for(int i=tid*(n/4);i<(tid+1)*(n/4);i++){
			parallel_sum = parallel_sum + array[i];
		}
	}

	// Parallel + Critical Directives
	#pragma omp parallel num_threads(4)
	{
		int tid=omp_get_thread_num();
		// cout<<tid<<"is\n";
		for(int i=tid*(n/4);i<(tid+1)*(n/4);i++){
			#pragma omp critical
        	{
            	parallel_critical_sum = parallel_critical_sum + array[i];
        	}	
		}
		
	}

	// Parallel + Critical + For Directives
	#pragma omp parallel for
	for (int i = 0; i < n; i++) 
    {
		#pragma omp critical
		parallel_for_critical_sum+=array[i];
    }
	
	// Parallel + For + Reduction Directives
	#pragma omp parallel for reduction (+:parallel_for_reductive_sum)
	for (int i=0;i<n;i++)
  	 	parallel_for_reductive_sum+=array[i];

	double formula_sum = (double)((n*(n-1))/2);

	cout << "N: " << n <<endl;
	cout << "Serailized Loop Sum: " << sum << endl;
	cout << "Parallel Directive Sum: "<<parallel_sum<<endl;
	cout << "Parallel Critical Directive Sum: "<<parallel_critical_sum<<endl;
	cout << "Parallel For Critical Sum: "<<parallel_for_critical_sum<<endl;
	cout << "Parallel For Reductive Sum: "<<parallel_for_reductive_sum<<endl;
	cout << "Formula Sum: " << formula_sum << endl;

	delete[] array;
	return 0;
}
