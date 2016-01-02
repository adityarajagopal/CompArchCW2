#ifndef MEM_SIM_BLOCK_H
#define MEM_SIM_BLOCK_H

#include <string>
#include <sstream> 
#include <vector> 
#include <iostream> 

struct Block
{
	bool dirty; 
	bool valid; 
	uint32_t tag; 
	std::vector<std::string> data; 

	Block(int num_words);	
	void write_block(std::string& block, unsigned bytes_per_word);
	void toString(std::string& block);
	friend std::ostream& operator<<(std::ostream& os, Block& block);
};

#endif