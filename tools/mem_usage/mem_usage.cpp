#include "mem_usage.hpp"

#include <iostream>

// comment or delete to disable
// otherwise extra heap memory would be used to track usage
#define ENABLE_MEMORY_USAGE

namespace lio
{
	size_t memory_count = 0;
	size_t memory_used = 0;
	size_t memory_peak = 0;

	void print_mem_usage(uint8_t args)
	{
		std::cout << "memory - ";
		if (args & USAGE)
			std::cout << "usage: " << memory_used << " ";
		if (args & COUNT)
			std::cout << "count: " << memory_count << " ";
		if (args & PEAK)
			std::cout << "peak: " << memory_peak << " ";
		std::cout << std::endl;
	}
}

#ifdef ENABLE_MEMORY_USAGE

#include <cstdlib>

void* operator new(size_t size)
{
	auto* ptr = (size_t*)malloc(size + sizeof(size_t));
	ptr[0] = size;
	lio::memory_used += size;
	++lio::memory_count;

	if (lio::memory_used > lio::memory_peak)
		lio::memory_peak = lio::memory_used;

	return (void*)&ptr[1];
}

void operator delete(void* mem_addr)
{
	auto* ptr = (size_t*)mem_addr;
	lio::memory_used -= ptr[-1];
	--lio::memory_count;

	free((void*)&ptr[-1]);
}

#endif