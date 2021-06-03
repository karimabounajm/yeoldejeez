

/* Insertion Sort
	Insertion Sort involves traversing a given array index by index, and at every new 
index moving the element in the index element by element until it reaches a defined
position form, usually with the value to its left less than it and the value to its
right less than it. Its elements are moved in traversals called swapping, which 
essentially involves swapping two adjacent elements in an effort to sort a given 
array in a specific way. It is O(N^2), with each "insertion" being O(N);
	It utilizes the fact that all the values within the array, as it is being sorted,
are themselves sorted, in the sense that when a new element is added to the end of
the sorted array for sorting, all the elements before it are themselves sorted, 
which means that once it reaches a position in which the value to its left is less
than it, all values to its left are non-increasing and less than it. the same 
applies to when a value to its right is greater than it, though to make things clear
it should be noted that when an element is added to the sorted array the elements
to its right, which are in the larger unsorted array, are themselves unsorted. obv;
	They are quite often used within more complex sorting algorithms as an auxiliary 
that sorts out sub arrays of discrete information sorted in a complex but efficient
way. 
*/

/* Shellsort
	Shellsort essentially involves the insertion sorting of elements with keys that 
are multiples of a given value, with the value decreasing with every pass over until
the entire array is sorted using insertion sort, at which point the array is well 
sorted anyways. This is also known as diminishing increment sort, with the increment
between elements that are sorted using insertion sort decreasing with every pass over
of the array. 
	Differing series of increments result in differing time complexities. Using 
Shell's suggested increments of N/2 still has a worst case of O(N^2).
	• Shell's incremenets of N/2, series prev/2, is O(N^2)
	• Hibbart's incremenets of 1, 3, 7, n^k - 1, is O(N^(3/2)) = O(N*sqrt(N))
	• Sedgewick's proposes a variety of increments, with some of his strategies 
	being O(N^(4/3)) = O(N*cube(N)), which is amazing;
*/


/* Heapsort
	Heapsort essentailly involves using buildheap with the elements of an array to 
to create a heap ADT that is sorted a paricular way, and then removing the top of 
it until the heap is empty, essentially removing all of its elements in order. what 
this means is, if it is a max heap then, if the element removed is placed at the
bottom of the heap (note that it is not technically in the heap, just where the bottom
would be when it was removed), then heapsort would sort in increasing order from min 
to max. This is because the maximum of the heap is always sent to the back. Same idea
for min heap. The buildheap for N elements is O(N) avg. This is followed by N O(logN)
deletions, which means that the total complexity is O(N*logN)
	They are known for their consistent runtimes, with the ADT operations involved
almost always being consistent and having minimal worst cases. 
*/


/* Mergesort
	Mergesort essentially involves the merging of two sorted arrays. The mechanism 
through which it does so is a series of comparisons from the beginning of each of 
the arrays, called a count. Whichever element evaluates to false based on the 
condition, ie is less than for sorting in increasing order or greater than for 
decreasing order, is added to the array being formed from the merging of the other
two, and the counter on the array from which that element is extracted is incremented,
and from there the process continues until all the elements in both arrays are all 
in the third array. A single merge sort of two arrays takes O(N), given that the sum
of elements in both arrays is N.
	Applying mergesort alone on an entire array of values involves recursively 
dividing the array into halves until it reaches arrays of length 1 (which are sorted 
by definition), and then merge sorting each of those arrays one by one until every 
array is sorted. This time complexity of this is determined through a telescoping 
relationship, and is O(N*logN)
	The runtime of mergesort, when compared with other O(N*logN) alternatives,
depends heavily on the relative costs of comparing elements and moving elements in the
array (and the temporary array). These costs are language dependent. In classic C++ 
generic sorts that involve copying objects can be expensive if the objects are large, 
but comparing objects often is relatively cheap because the compiler aggressively 
performs inline optimization.
*/
	


/* Quick Sort
	Quick sort essentially involves sorting the elements of an array as greater than
or equal to a particular element chosen, with each sort creating two smaller arrays
that are bounded from the right and left respectively by the pivot, or the element
by which they are created. Because the pivot is defined by having all the elements
in the array less than it to its left and greater than it to its right, a pivot is 
always in the correct place in the array. Quick sort continues to sort smaller and 
smaller arrays by pivots determiend by an element in them until every element is 
sorted, as each element will have all values to its left less than it and all 
elements to its right greater than it;
	Note that most implementations quick sort have what is called a cut-off value, or 
a length at which an array won't be sorted using the less-than greater-than approach
of an element, but rather will be sorted via insert sort, or manually. This is 
because at a certain, small, length the insert sort is faster, with quick sort being
used to effectively break arrays into these small lengths at a rapid pace using 
simple code to enable the use of insert sorts;
	Note that the chosen method for deciding the pivot in a given array that is being 
subjected to quick sort is to take the median, or middle value, of the left-most 
element in the array, the rightmost element in the array, and the element in the 
middle, the index of which is determined by the sum of the indices of the rightmost
and leftmost elements divided by two. pretty intuitive.
*/