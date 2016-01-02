#include "mem_sim_ram.h"

RAM::RAM(unsigned i_blockSize, unsigned address_size, unsigned bytes_per_word) : 
	blockSize(i_blockSize), 
	wordSize(bytes_per_word)
{
	unsigned num_bytes = pow(2, address_size);
	for(int i=0; i<num_bytes; i++)
		memory.push_back("00");
}

void RAM::write(unsigned address, std::string& data)
{
	if(data.size() != (2*wordSize*blockSize))
	{/*throw error that blockSize is voilated*/}

	int byte = 0;
	int i = 0;  
	std::stringstream ss; 
	std::string tmp; 
	while(byte < 2*wordSize*blockSize)
	{	
		ss << data[byte] << data[byte+1];
		tmp = ss.str(); 
		memory[address+i] = tmp; 
		i++; 
		byte+=2; 
		ss.str("");
	}
}

void RAM::read(unsigned address, std::string& data)
{
	std::stringstream ss; 
	int i=0; 
	
	unsigned blockAddress = address/(wordSize * blockSize); 
	address = blockAddress*wordSize*blockSize;
	
	while(i < (wordSize*blockSize))
	{
		ss << memory[address + i];
		i++; 
	}
	data = ss.str(); 
}

std::ostream& operator<<(std::ostream& os, RAM& ram)
{
	os << "Address" << "  |  " << "Byte" << std::endl;
	for(int i=0; i<ram.memory.size(); i++)
		os << std::hex << i << "        |   " << ram.memory[i] << std::endl;
	return os; 
}