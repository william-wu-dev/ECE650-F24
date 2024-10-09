// C++ program to illustrate the race conditions 
// https://www.geeksforgeeks.org/std-mutex-in-cpp/
#include <iostream> 
#include <thread> 
#include <exception>

using namespace std; 


// function to increment the number 
void increment(int* number){ 
	
	// increment number by 1 for 1000000 times 
	for(int i=0; i<1000000; i++){ 
		*number = *number + 1; 
	} 
} 

int main() 
{ 
	try {
		int number = 0; 

		// Create thread t1 to perform increment() 
		thread t1(increment, &number); 
		
		// Create thread t2 to perform increment() 
		thread t2(increment, &number); 
		
		// Start both threads simultaneously 
		t1.join(); 
		t2.join(); 
		
		// Print the number after the execution of both threads 
		cout << "Number after execution of t1 and t2 is " << number << endl; 
	} catch (exception& e) {
		cerr << "Error: " << e.what() << endl;
	}
	
	return 0; 
} 
