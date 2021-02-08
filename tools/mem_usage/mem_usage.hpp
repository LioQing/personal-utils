#pragma once

#include <cstdint>

namespace lio
{
	enum MemArgs : uint8_t
	{
		USAGE = 0b001,
		COUNT = 0b010,
		PEAK = 0b100
	};

	extern size_t memory_count;
	extern size_t memory_used;
	extern size_t memory_peak;

	void PrintMemUsage(uint8_t args = USAGE);
}