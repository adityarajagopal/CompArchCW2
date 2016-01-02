#include "mem_sim_block.h"

Block::Block(int num_words) :
	dirty(false),
	valid(false), 
	tag(0)
{
	for(int i=0; i<num_words; i++)
		data.push_back("0");
}

void Block::write_block(std::string& block, unsigned bytes_per_word)
{
	int block_size = data.size(); 
	std::stringstream ss; 

	int i=0; 
	for(int word = 0; word < (block_size*2*bytes_per_word); word = word + (2*bytes_per_word))
	{
		for(int hbyte = word; hbyte < word + (2*bytes_per_word); hbyte++)
			ss << block[hbyte]; 
		data[i] = ss.str(); 
		ss.str("");
		i++;
	}
}

void Block::toString(std::string& block)
{
	std::stringstream ss; 
	for(int i=0; i<data.size(); i++)
		ss << data[i]; 
	block = ss.str(); 
}

std::ostream& operator<<(std::ostream& os, Block& block)
{
	os << "dirty" << " | " << "valid" << " | " << "tag" << " | " << "data" << "      " << std::endl;
	os << block.dirty << "     | " << block.valid << "     | " << std::hex << block.tag << "   | ";
	for(int i=0; i<block.data.size(); i++)
		os << block.data[i] << " ";
	os << std::endl;

	return os;
}