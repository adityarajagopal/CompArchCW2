#ifndef MEM_SIM_SET_H
#define MEM_SIM_SET_H

#include "mem_sim_block.h"
#include <list>

struct Set
{
	std::list<unsigned> queue; 
	std::vector<Block> block;

	Set(int num_blocks, int words_per_block, int bytes_per_word);
	void update_queue(unsigned new_block);
	friend std::ostream& operator<<(std::ostream& os, Set& set);
};

#endif