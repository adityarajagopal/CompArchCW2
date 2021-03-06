# Discussion of Computer Architecture CW2

## Development 

The initial approach to development was to create a separate class for RAM, Cache, Set, and Block. However, I realised that defining Set and Block as a class of their own would mean hiding their internal data structures from the overlying Cache class. This was counter productive as a Cache should be able to have full knowledge and control of the sets and blocks they are made of. So, I used a "struct" to define the data structure for a Set and a Block so that all their internal data structures are public and kept the Cache and the RAM as two separate classes to encapsulate them from each other. 

Another design decision that I made was to avoid dynamically allocating memory as both the size and contents of the RAM and Cache are pre-defined. Hence, I use vectors, which are initialised on reading the input parameters, to represent both the cache and the RAM.

The other main design decision that I made was to store the data in memory as strings as opposed to an unsigned long long int. This is because the string data type is flexible and can accommodate for any length of data specified in the input as opposed to an unsigned long long int data type that can only hold data that is 8 bytes long. 

## Testing
 
For testing my cache, I used the tests shown in the submitted file "testcases.txt". To run these tests I used the shell script "makefile.sh" which has also been submitted. The tests shown, test direct-mapped, 2-way associative, 4-way associative and fully associative caches. Some corner cases I looked for while testing include whether the LRU replacement scheme was implemented correctly, and if the write-misses were optimised in single-word blocks. 

##Debugging

To facilitate debugging of my cache, I overloaded the "<<" operator for the cache class and the RAM class such that passing an instance of a cache or RAM to "cout" will print out the current status of all the cache and memory locations. Placing a "debug-req" after every inpnut command allowed me to run the code and get an output that checks if the cache and memory are functioning as required.  
   




  
