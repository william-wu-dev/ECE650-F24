// C++ program to illustrate the thread synchronization using mutex 
// https://www.geeksforgeeks.org/std-mutex-in-cpp/
#include <iostream> 
#include <thread> 
#include <exception>

using namespace std; 

// import mutex from C++ standard library 
#include <mutex> 

// Create object for mutex 
mutex mtx; 

// function to increment the number 
void increment(int* number){ 
	
	// Lock the thread using lock 
	mtx.lock(); 
	
	// increment number by 1 for 1000000 times 
	for(int i=0; i<1000000; i++){ 
		*number = *number + 1; 
	} 
	
	// Release the lock using unlock() 
	mtx.unlock(); 
} 

int main() 
{ 
	// Shared resource 
	int number = 0; 
	
	try {
		// Create thread t1 to perform increment() 
		thread t1(increment, &number); 
		
		// Create thread t2 to perform increment() 
		thread t2(increment, &number); 
		
		// Start both threads simultaneously 
		t1.join(); 
		t2.join(); 
		
		// Print the number after the execution of both threads 
		std::cout<<"Number after execution of t1 and t2 is "<<number << endl; 
	} catch (exception& e) {
		cerr << "Error: " << e.what() << endl;
	}
	
	return 0; 
} 
