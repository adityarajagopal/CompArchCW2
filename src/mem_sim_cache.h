#ifndef MEM_SIM_CACHE_H
#define MEM_SIM_CACHE_H

#include "mem_sim_set.h"
#include "mem_sim_ram.h"

class Cache
{
private: 
	unsigned bytes_per_word; 
	unsigned words_per_block; 
	unsigned blocks_per_set; 
	unsigned mem_read_time; 
	unsigned mem_write_time; 
	RAM& ram; 
	std::vector<Set> set;

public: 
	Cache(RAM& memory, unsigned num_sets, unsigned blocks_per_set, unsigned words_per_block, unsigned bytes_per_word, unsigned mem_read_time, unsigned mem_write_time);
	bool read(unsigned address, std::string& data, unsigned& setIndex, unsigned& timeTaken);
	bool write(unsigned address, std::string& data, unsigned& setIndex, unsigned& timeTaken);
	void flush(unsigned& timeTaken);
	friend std::ostream& operator<<(std::ostream& os, Cache& cache); 
};

#endif