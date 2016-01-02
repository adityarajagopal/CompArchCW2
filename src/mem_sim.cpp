#include "mem_sim_cache.h"
#include "mem_sim_ram.h"

#include <cstdlib> 
#include <string>

int main(int argc, char *argv[]) 
{
	unsigned address_bits, bytes_per_word, words_per_block, blocks_per_set, sets_per_cache, hit_time, mem_read_time, mem_write_time;

	address_bits = unsigned(atoi(argv[1])); 
	bytes_per_word = unsigned(atoi(argv[2])); 
	words_per_block = unsigned(atoi(argv[3]));  
	blocks_per_set = unsigned(atoi(argv[4])); 
	sets_per_cache = unsigned(atoi(argv[5])); 
	hit_time = unsigned(atoi(argv[6])); 
	mem_read_time = unsigned(atoi(argv[7])); 
	mem_write_time = unsigned(atoi(argv[8])); 

	RAM r1(words_per_block, address_bits, bytes_per_word);
	Cache c1(r1, sets_per_cache, blocks_per_set, words_per_block, bytes_per_word, mem_read_time, mem_write_time);

	while(1)
	{
		std::string command;

		std::cin >> command; 
	
		//check for end of file
		if(command == "")
			break;
		//if read request
		else if(command == "read-req")
		{
			unsigned address; 
			std::cin >> address;

			std::string data; 
			unsigned setIndex, timeTaken;
			bool read = c1.read(address, data, setIndex, timeTaken);

			std::stringstream ss; 
			if(read)
				ss << "read-ack " << int(setIndex) << " " << "hit " << int(timeTaken + hit_time) << " " << data << std::endl;
			else
				ss << "read-ack " << int(setIndex) << " " << "miss " << int(timeTaken + hit_time) << " " << data << std::endl;

			std::cout << ss.str(); 
		}
		//if write request
		else if(command == "write-req")
		{
			unsigned address;
			std::cin >> address;

			std::string data; 
			std::cin >> data; 		

			unsigned setIndex, timeTaken; 
			bool write = c1.write(address, data, setIndex, timeTaken); 

			std::stringstream ss; 
			if(write)
				ss << "write-ack " << int(setIndex) << " " << "hit " << int(timeTaken + hit_time) << std::endl;
			else
				ss << "write-ack " << int(setIndex) << " " << "miss " << int(timeTaken + hit_time) << std::endl;

			std::cout << ss.str(); 
			// r1.write(address, data);
		}
		//if flush request 
		else if(command == "flush-req")
		{
			unsigned timeTaken = 0;
			c1.flush(timeTaken); 
			std::cout << "flush-ack " << int(timeTaken) << std::endl;
		}
		//if debug request 
		else if(command == "debug-req")
		{
			int mode;
			std::cin >> mode;
			std::cout << "debug-ack-begin" << std::endl;
			std::cout << c1;
			if(mode)
				std::cout << r1;
			std::cout << "debug-ack-end" << std::endl;
		}
		//if comments 
		else if(command == "#")
		{
			std::string ignore;
			std::getline(std::cin, ignore);
		}
	} 	

	return 0; 
}