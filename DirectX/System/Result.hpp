#pragma once

namespace System
{
	class Result
	{
	public:

		typedef unsigned int GUID;

		Result();
		~Result();

		Result(GUID code);

		GUID code() const { return m_code; }

		bool operator == (const GUID &code) const { return (this->m_code == code); }

		bool operator != (const GUID &code) const { return (this->m_code != code); }

		Result operator &= (const Result &r) const;

	private:
		GUID m_code;
	};

	#define DECLARE_RESULT_CODE(resultName) extern Result::GUID resultName;
	#define DEFINE_RESULT_CODE(resultName) Result::GUID resultName = Result::GenerateGUID();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Common result types
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DECLARE_RESULT_CODE(Undefined);

	DECLARE_RESULT_CODE(Failure);

	DECLARE_RESULT_CODE(Success);

	DECLARE_RESULT_CODE(NotImplemented);

	DECLARE_RESULT_CODE(NotInitialized);

	DECLARE_RESULT_CODE(NotFound);

	DECLARE_RESULT_CODE(InvalidArgs);

	DECLARE_RESULT_CODE(OutOfMemory);


	// helper shortcuts
	#define Succeeded(result) (Success == (result).code())
	#define Failed(result) (Success != (result).code())
	#define CombinedResult(res0, res1) (Failed(res0) || Failed(res1) ? Result(Failure) : Result(Success))

}