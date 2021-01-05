//
//  main.cpp
//  firstCPP
//
//  Created by Karim Abou Najm on 12/27/20.
//  Copyright © 2020 Karim Abou Najm. All rights reserved.
//

#include <iostream>
#include <cmath>


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n"; // << endl; can be used instead of \n
    std::cout << "Hello, World!" << std::endl; // functionally equivalent
    // "using namespace std" allows us to avoid doing std:: every time
    
// getting strings from users is done through
//    getline(cin, string variable);`
    return 0;
}


//  constructor functions are cool, use them when declaring a class
//  is there a way to allocate memory for pointers of a class within the constructor
//  function of the class?

//  constructor functions with the same name but different parameters can be created, which
//  is a good thing because it means that we won't NEED those parameters in the initialization
//  of the class

//  can make classes cooler/more intuitive by including in the constructor functions request for
//  inputs from a user if certain parameters are not defined

//  private parts of a class cannot be changed outside of constructor functions or stuff within
//  the class itself; everything inside it can only be accessed from within the class and functions
//  the constructor functions that are inside it
//  constructor functions in the public part of a class can be called by class.function(parameters) to
//  modify it as you wish based on the way the function is written

//  inheritance involves taking the elements of one class in a new one
//  has a simple general structure of

/*
 
 class c1 {
    public:
        code
    }
 
 class c2 {
     public:
         code
    }
 class inheritorClass: public c1, public c2 {
     public:
         code
    }
    the inheritor class essentially inherits everything that the
    classes taken have; this includes elements, constructors,
    anything and everything that is public in those class'
 */
