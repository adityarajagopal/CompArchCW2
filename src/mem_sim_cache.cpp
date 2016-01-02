#include "mem_sim_cache.h"

Cache::Cache(RAM& memory, unsigned num_sets, unsigned blocks_per_set, unsigned words_per_block, unsigned bytes_per_word, unsigned mem_read_time, unsigned mem_write_time) : 
	bytes_per_word(bytes_per_word), 
	words_per_block(words_per_block), 
	blocks_per_set(blocks_per_set), 
	mem_read_time(mem_read_time), 
	mem_write_time(mem_write_time),
	ram(memory)
{
	for(int i=0; i<num_sets; i++)
	{
		set.push_back(Set (blocks_per_set, words_per_block, bytes_per_word));
	}
}

bool Cache::read(unsigned address, std::string& data, unsigned& setIndex, unsigned& timeTaken)
{
	unsigned blockAddress = address/(bytes_per_word * words_per_block); 
	unsigned blockOffset = (address % (bytes_per_word * words_per_block)) / bytes_per_word;
	setIndex = blockAddress % set.size(); 

	unsigned invalid = -1; 

	//check if data is in cache
	for(int i=0; i<set[setIndex].block.size(); i++)
	{		
		if(!set[setIndex].block[i].valid)
			invalid = i; 

		if((set[setIndex].block[i].tag == blockAddress) && set[setIndex].block[i].valid)
		{
			set[setIndex].update_queue(i);
			data = set[setIndex].block[i].data[blockOffset]; 
			timeTaken = 0; 
			return true; 
		}
	}

	//if invalid block available, read data from memory into that
	if(invalid != -1)
	{
		set[setIndex].update_queue(invalid);

		set[setIndex].block[invalid].dirty = false; 
		set[setIndex].block[invalid].valid = true; 
		set[setIndex].block[invalid].tag = blockAddress; 
		
		std::string i_block; 
		ram.read(address, i_block);
  		set[setIndex].block[invalid].write_block(i_block, bytes_per_word);

  		data = set[setIndex].block[invalid].data[blockOffset];
  		timeTaken = mem_read_time; 
	}

	//if no invalid block available, evict LRU block 
	if(invalid == -1)
	{
		int LRU_block = set[setIndex].queue.back(); 
		set[setIndex].update_queue(LRU_block); 

		//check if block is dirty 
		if(set[setIndex].block[LRU_block].dirty)
		{
			//write back cache to memory
			std::string toWrite;
			unsigned add_toWrite = (set[setIndex].block[LRU_block].tag) * (bytes_per_word * words_per_block);
			set[setIndex].block[LRU_block].toString(toWrite);  
			ram.write(add_toWrite, toWrite);

			//read from memory required data 
			std::string i_block; 
			ram.read(address, i_block);
	  		set[setIndex].block[LRU_block].write_block(i_block, bytes_per_word);

	  		data = set[setIndex].block[LRU_block].data[blockOffset];
	  		timeTaken = mem_read_time + mem_write_time; 
		}
		else
		{
			//read from memory required data 
			std::string i_block; 
			ram.read(address, i_block);
	  		set[setIndex].block[LRU_block].write_block(i_block, bytes_per_word);

	  		data = set[setIndex].block[LRU_block].data[blockOffset];
	  		timeTaken = mem_read_time; 
		}

		set[setIndex].block[LRU_block].dirty = false; 
		set[setIndex].block[LRU_block].valid = true; 
		set[setIndex].block[LRU_block].tag = blockAddress; 
	}
	return false;
}

bool Cache::write(unsigned address, std::string& data, unsigned& setIndex, unsigned& timeTaken)
{
	unsigned blockAddress = address/(bytes_per_word * words_per_block); 
	unsigned blockOffset = (address % (bytes_per_word * words_per_block)) / bytes_per_word;
	setIndex = blockAddress % set.size(); 

	unsigned invalid = -1;
	
	//check if data is already in cache
	for(int i=0; i<set[setIndex].block.size(); i++)
	{
		if(!set[setIndex].block[i].valid)
			invalid = i; 

		if((set[setIndex].block[i].tag == blockAddress) && set[setIndex].block[i].valid)
		{
			set[setIndex].update_queue(i);
			set[setIndex].block[i].dirty = true;
			set[setIndex].block[i].data[blockOffset] = data;
			timeTaken = 0; 
			return true;
		}
	}

	//if invalid block available, write into that block
	if(invalid != -1)
	{
		timeTaken = 0; 
		set[setIndex].block[invalid].dirty = true; 
		set[setIndex].block[invalid].valid = true; 
		set[setIndex].block[invalid].tag = blockAddress; 
		set[setIndex].update_queue(invalid);
		if(words_per_block > 1)
		{
			//read from memory required block
			std::string i_block; 
			ram.read(address, i_block);
			timeTaken += mem_read_time;
  			set[setIndex].block[invalid].write_block(i_block, bytes_per_word);
	  	}
  		set[setIndex].block[invalid].data[blockOffset] = data;
	}

	//if no invalid block availabe, evict LRU block and write there
	if(invalid == -1)
	{
		int LRU_block = set[setIndex].queue.back(); 
		set[setIndex].update_queue(LRU_block); 
		timeTaken = 0; 

		//check if block is dirty 
		if(set[setIndex].block[LRU_block].dirty)
		{
			//write back cache to memory
			std::string toWrite;
			unsigned add_toWrite = (set[setIndex].block[LRU_block].tag) * (bytes_per_word * words_per_block);
			set[setIndex].block[LRU_block].toString(toWrite);  
			ram.write(add_toWrite, toWrite);
			timeTaken += mem_write_time;

			if(words_per_block > 1)
			{
				//read required block in 
				std::string i_block; 
				ram.read(address, i_block);
				timeTaken += mem_read_time;
		  		set[setIndex].block[LRU_block].write_block(i_block, bytes_per_word);
		  	}
	  		
	  		set[setIndex].block[LRU_block].data[blockOffset] = data; 
		}
		else 
		{
			if(words_per_block > 1)
			{
				//read from memory required block
				std::string i_block; 
				ram.read(address, i_block);
	  			timeTaken = mem_read_time; 
		  		set[setIndex].block[LRU_block].write_block(i_block, bytes_per_word);
		  	}

	  		set[setIndex].block[LRU_block].data[blockOffset] = data;
		}

		set[setIndex].block[LRU_block].dirty = true; 
		set[setIndex].block[LRU_block].valid = true; 
		set[setIndex].block[LRU_block].tag = blockAddress; 
	}
	return false;
}

void Cache::flush(unsigned& timeTaken)
{
	int num_sets = set.size();
	for(int setIndex=0; setIndex<num_sets; setIndex++)
	{
		for(int blockIndex=0; blockIndex < blocks_per_set; blockIndex++)
		{
			if(set[setIndex].block[blockIndex].dirty && set[setIndex].block[blockIndex].valid)
			{
				std::string toWrite;
				unsigned add_toWrite = (set[setIndex].block[blockIndex].tag) * (bytes_per_word * words_per_block);
				set[setIndex].block[blockIndex].toString(toWrite);  
				ram.write(add_toWrite, toWrite);
				timeTaken += mem_write_time;
				set[setIndex].block[blockIndex].dirty = false;
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, Cache& cache)
{
	for(int i=0; i<cache.set.size(); i++)
	{
		os << i << std::endl;
		os << cache.set[i];
	}
	return os;
}