huffman.out : huffman.o  
	g++ -ansi -Wall -g -o huffman.out huffman.o  

huffman.o : huffman.cpp BinaryTree.h BinaryHeap.h 
	g++ -ansi -Wall -Wno-char-subscripts -g -c huffman.cpp

clean : 
	rm -f huffman.out huffman.o   
