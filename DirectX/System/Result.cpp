#include "stdafx.h"
#include "Result.hpp"

namespace System
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Result
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Result::Result() : m_code(Undefined)
	{
	}


	Result::~Result()
	{
	}
	
	Result::Result(GUID code)
		: m_code(code)
	{
	}

	Result Result::operator &= (const Result &r) const
	{
		return CombinedResult(*this, r);
	}

}
