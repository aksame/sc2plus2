// Taken from https://github.com/zsrtp/GC-Randomizer/blob/stable/source/patch.cpp
#include "sc2/patch.h"
#include "sc2/gc/OSCache.h"
#include <stdint.h>

namespace mod::patch
{
	void writeBranch(void* ptr, void* destination)
	{
		uint32_t branch = 0x48000000; // b
		writeBranchMain(ptr, destination, branch);
	}

	void writeBranchLR(void* ptr, void* destination)
	{
		uint32_t branch = 0x48000001; // bl
		writeBranchMain(ptr, destination, branch);
	}

	void writeBranchMain(void* ptr, void* destination, uint32_t branch)
	{
		uint32_t delta = reinterpret_cast<uint32_t>(destination) - reinterpret_cast<uint32_t>(ptr);

		branch |= (delta & 0x03FFFFFC);

		uint32_t* p = reinterpret_cast<uint32_t*>(ptr);
		*p = branch;

		gc::os_cache::DCFlushRange(ptr, sizeof(uint32_t));
		gc::os_cache::ICInvalidateRange(ptr, sizeof(uint32_t));
	}
}
