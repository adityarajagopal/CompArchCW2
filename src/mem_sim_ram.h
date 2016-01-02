#ifndef MEM_SIM_RAM_H
#define MEM_SIM_RAM_H

#include <cmath>
#include <vector>
#include <string> 
#include <sstream> 
#include <iostream> 

class RAM 
{
private: 
	unsigned blockSize; 
	unsigned wordSize; 
	std::vector<std::string> memory; 

public: 
	RAM(unsigned i_blockSize, unsigned address_size, unsigned bytes_per_word);
	void write(unsigned address, std::string& data);
	void read(unsigned address, std::string& data);

	friend std::ostream& operator<<(std::ostream& os, RAM& ram);
};

#endif