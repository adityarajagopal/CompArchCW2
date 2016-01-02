#include "mem_sim_set.h"

Set::Set(int num_blocks, int words_per_block, int bytes_per_word)
{
	for(int i=0; i<num_blocks; i++)
	{
		block.push_back(Block (words_per_block));
	}
}

void Set::update_queue(unsigned new_block)
{
	bool found = false; 
	std::list<unsigned>::iterator it; 
	for(it = queue.begin(); it != queue.end(); ++it)
	{
		if(*it == new_block)
		{
			found = true;
			if(it != queue.begin())
			{
				unsigned tmp = *it; 
				queue.erase(it); 
				queue.push_front(tmp); 
			}
		}
	}
	if(found == false)
		queue.push_front(new_block);
}

std::ostream& operator<<(std::ostream& os, Set& set)
{
	for(int i=0; i<set.block.size(); i++)
		os << set.block[i];
	os << std::endl;
	return os;
}
