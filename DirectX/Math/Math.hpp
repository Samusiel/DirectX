#pragma once

namespace Math
{

	inline size_t ComputeHash(const cbyte *buffer, const size_t sizeInBytes)
	{
#ifdef UR_PLATFORM_x64
		const ur_size fnv_offset_basis = 14695981039346656037;
		const ur_size fnv_prime = 1099511628211;
#else
		const size_t fnv_offset_basis = 2166136261;
		const size_t fnv_prime = 16777619;
#endif
		size_t hash = fnv_offset_basis;
		const cbyte *p_octet = buffer;
		const cbyte *p_end = buffer + sizeInBytes;
		for (; p_octet != p_end; ++p_octet)
		{
			hash ^= *p_octet;
			hash *= fnv_prime;
		}
		return hash;
	}

}