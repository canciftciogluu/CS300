#include <iostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

template <class Comparable>
inline void swaping( Comparable & obj1,Comparable & obj2 )
{
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;}


template <class Comparable>
void insertionSort(vector<Comparable> &b) {
    // Using range-based for loop for the outer loop
    for (size_t p = 1; p < b.size(); p++) {
        Comparable tmp = b[p];
        size_t j;

        // Refactored inner loop using while instead of for
        j = p;
        while (j > 0 && tmp.word < b[j - 1].word) {
            b[j] = b[j - 1];
            j--;
        }
        b[j] = tmp;
    }
}

template <class Comparable>void heapsort(vector<Comparable> & a)

{
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
        percDown(a, i, a.size());
    // sort
    for (int j = a.size() -1; j >0; j--)
    {
        swaping(a[0], a[j]); // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }}

inline int leftChild(int i )
{
    return 2*i+1;}


template <class Comparable>
void percDown(vector<Comparable> &a, int i, int n) {
    int child;
    Comparable tmp = a[i];

    // Refactored loop using while instead of for
    while (leftChild(i) < n) {
        child = leftChild(i);
        if (child != n - 1 && a[child].word < a[child + 1].word) {
            child++;
        }
        if (a[child].word > tmp.word) {
            a[i] = a[child];
            i = child; // Move down to the child
        } else {
            break; // Break if heap property is satisfied
        }
    }
    a[i] = tmp; // Put the temporary value in its final position
}

template <class Comparable> void mergeSort( vector<Comparable>  &a )
{
    vector<Comparable> tmpArray( a.size( ) );
    mergeSort( a, tmpArray, 0, a.size( ) - 1 );
    //return a;
}



template <class Comparable>void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right )
{
    if ( left < right )
    {
        int center = ( left + right ) / 2;
        mergeSort( a, tmpArray, left, center );
        mergeSort( a, tmpArray, center + 1, right );
        merge( a, tmpArray, left, center + 1, right );}}










template <class Comparable>
void merge(vector<Comparable> &a, vector<Comparable> &tmpArray, int leftPos, int rightPos, int rightEnd) {
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    // Refactored main loop using for loop
    for (; leftPos <= leftEnd && rightPos <= rightEnd; tmpPos++) {
        if (a[leftPos].word <= a[rightPos].word) {
            tmpArray[tmpPos] = a[leftPos++];
        } else {
            tmpArray[tmpPos] = a[rightPos++];
        }
    }

    // Refactored loop for copying the rest of the first half
    for (; leftPos <= leftEnd; tmpArray[tmpPos++] = a[leftPos++]);

    // Refactored loop for copying the rest of the right half
    for (; rightPos <= rightEnd; tmpArray[tmpPos++] = a[rightPos++]);

    // Copying tmpArray back
    for (int i = 0; i < numElements; rightEnd--, i++) {
        a[rightEnd] = tmpArray[rightEnd];
    }
}

template <class Comparable>void quicksort( vector<Comparable> & a, string type )
{
    if (type=="median")
        quicksort( a, 0, a.size( ) - 1,"median" );
    else if (type=="first")
        quicksort( a, 0, a.size( ) - 1,"first" );
    else if (type=="random")
        quicksort( a, 0, a.size( ) - 1,"random" );
}

template <class Comparable>const Comparable & median3( vector<Comparable> &a,int left, int right )
{
    int center = ( left + right ) / 2;
    if ( a[ center ].word < a[ left ].word )
        swaping( a[ left ], a[ center ] );

    if ( a[ right ].word < a[ left ].word )
        swaping( a[ left ], a[ right ] );

    if ( a[ right ].word < a[ center ].word )
        swaping( a[ center ], a[ right ] );
    // Place pivot at position right - 1
    swaping( a[ center ], a[ right - 1 ] );
    return a[ right - 1 ];
}

template <class Comparable>
void quicksort(vector<Comparable> &a, int left, int right) {
    if (left + 10 <= right) {
        Comparable pivot = median3(a, left, right);
        int i = left, j = right - 1;

        // Refactored while loops
        while (true) {
            while (a[++i].word < pivot.word);
            while (pivot.word < a[--j].word);

            if (i < j) {
                swaping(a[i], a[j]);
            } else {
                break;
            }
        }

        swaping(a[i], a[right - 1]); // Restore pivot
        quicksort(a, left, i - 1); // Sort small elements
        quicksort(a, i + 1, right); // Sort large elements
    } else {
        insertionSort(a, left, right); // Do an insertion sort on the subarray
    }
}

template <class Comparable>
void quicksortF( vector<Comparable>  &a, int left, int right)//,string type )

{

	if ( left + 10 <= right )

	{
		Comparable pivot=a[left] ;
		
		//(internal) quicksort
		// Begin partitioning
		

		int i = left, j = right - 1;

		for ( ; ; )

		{

			while ( a[ ++i ].word < pivot.word ) { }

			while ( pivot.word < a[ --j ].word ) { }

			if ( i < j )

				swaping( a[ i ], a[ j ] );

			else

				break;

		}

		swaping( a[ i ], a[ right - 1 ] ); // Restore pivot

		quicksort( a, left, i - 1 );//,type); // Sort small elements

		quicksort( a, i + 1, right);//,type ); // Sort large elements

	}

	else // Do an insertion sort on the subarray

		insertionSort( a, left, right );

	///return a;
}

template <class Comparable>
void quicksortR( vector<Comparable>  &a, int left, int right) //,string type )

{
	
	//int rand_num= rand()%right;
	//int rand_num = left + rand() % (right - left);
	if ( left + 10 <= right ) {
		Comparable pivot= a[rand()%right+1];
		
	int i = left, j = right - 1;

		for ( ; ; )

		{

			while ( a[ ++i ].word < pivot.word ) { }

			while ( pivot.word < a[ --j ].word ) { }

			if ( i < j )

				swaping( a[ i ], a[ j ] );

			else

				break;

		}

		swaping( a[ i ], a[ right - 1 ] ); // Restore pivot

		quicksort( a, left, i - 1);// ,type); // Sort small elements

		quicksort( a, i + 1, right);//,type ); // Sort large elements

	}

	else // Do an insertion sort on the subarray

		insertionSort( a, left, right );

	///return a;
}



template <class Comparable>void insertionSort( vector<Comparable> & a, int left, int right ){
    for ( int p = left + 1; p <= right; p++ )
    {
        Comparable tmp = a[ p ];
        int j;
        for ( j = p; j > left && tmp.word < a[ j - 1 ].word; j-- )
            a[ j ] = a[ j - 1 ];
        a[ j ] = tmp;}}

template <class Comparable>
void merge2(vector<Comparable> &arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Creating temporary vectors for left and right subarrays
    vector<Comparable> L, R;
    L.reserve(n1);
    R.reserve(n2);

    // Copying data to temp vectors L and R using range-based for loop
    for (int index = 0; index < n1; ++index) {
        L.push_back(arr[l + index]);
    }
    for (int index = 0; index < n2; ++index) {
        R.push_back(arr[m + 1 + index]);
    }

    // Merging the temp arrays back into arr[l..r]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = l; // Initial index of merged subarray

    // Modified while loop for merging
    for (; i < n1 && j < n2; ++k) {
        if (L[i].word <= R[j].word) {
            arr[k] = L[i++];
        } else {
            arr[k] = R[j++];
        }
    }

    // Copy the remaining elements of L, if there are any
    for (; i < n1; ++i, ++k) {
        arr[k] = L[i];
    }

    // Copy the remaining elements of R, if there are any
    for (; j < n2; ++j, ++k) {
        arr[k] = R[j];
    }
}

template <class Comparable>void mergeSort2(vector<Comparable> & arr, int l, int r){
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
        // Sort first and second halves
        mergeSort2(arr, l, m);
        mergeSort2(arr, m + 1, r);
        merge2(arr, l, m, r);}}

template <class Comparable>
vector<Comparable> quickSortG(vector<Comparable> arr, int low, int high,string type){
    if (low < high){
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high,type);
  
        // Separately sort elements before
        // partition and after partition
        arr=quickSortG(arr, low, pi - 1,type);
        arr=quickSortG(arr, pi + 1, high,type);}
    return arr;}
  

template<class Comparable>
int partition(vector<Comparable> &arr, int low, int high) {
    Comparable pivot = arr[high]; // pivot
    int i = low; // Start from the low index

    // Refactored for loop using while structure
    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j].word <= pivot.word) {
            if (i != j) {
                swaping(arr[i], arr[j]);
            }
            i++;
        }
    }

    swaping(arr[i], arr[high]); // Swap pivot into correct place
    return i; // Pivot index
}
  
// Generates Random Pivot, swaps pivot with
// end element and calls the partition function
template<class Comparable>
int partition_r(vector<Comparable> &arr, int low, int high)
{
    // Generate a random number in between low .. high
    srand(time(NULL));
    int random = low + rand() % (high - low);
    // Swap A[random] with A[high]
    swaping(arr[random], arr[high]);
    return partition(arr, low, high);}

/* The main function that implements QuickSort 
arr[] --> Array to be sorted, 
low --> Starting index, 
high --> Ending index */
template<class Comparable>
void quickSortrandom(vector<Comparable> &arr, int low, int high){
    if (low < high) {
        /* pi is partitioning index, arr[p] is now at right place */
        int pi = partition_r(arr, low, high);
        // Separately sort elements before partition and after partition
        quickSortrandom(arr, low, pi - 1);
        quickSortrandom(arr, pi + 1, high);}}
