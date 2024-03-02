#include "period.hpp"
using namespace Period;


namespace TestPeriodNoError
{
	void test(const std::string& registerStr, std::string& polynomial, bool flag)
	{
		auto positionBitsFeedback = flag == true ? Util::ParsePolynomialBitsFeedBack(polynomial) : Util::GetInfoBitsFeedBackFromPolynomial(polynomial);
		NotError noErr;
		BaseError* typeError = &noErr;
		Period::BOOLVALUE value = Period::BOOLVALUE::FALSE;
		Period::CalcPeriod(registerStr, positionBitsFeedback, value, 0, typeError);
		std::cout << "\n";
	}
}

namespace TestPeriodErrorCellMemory
{
	void test(const std::string  & registerStr, std::string & polynomial, bool flag)
	{
		auto positionBitsFeedback = flag == true ? Util::ParsePolynomialBitsFeedBack(polynomial) : Util::GetInfoBitsFeedBackFromPolynomial(polynomial);
		ErrorMemomryCell e;
		BaseError * typeError = &e;

		for (size_t i = 0; i < 2; ++i)
		{
			for (size_t j = 0; j < registerStr.size(); ++j)
			{
				Period::BOOLVALUE value = Period::BOOLVALUE::FALSE;
				if (i != 0)
				{
					value = Period::BOOLVALUE::TRUE;
				}
				Period::CalcPeriod(registerStr, positionBitsFeedback, value, j, typeError);
			}
		}
		std::cout << "\n";
	}

}

namespace TestPeriodErrorChain
{
	void test(const std::string& registerStr, std::string& polynomial, bool flag)
	{
		auto positionBitsFeedback = flag == true ? Util::ParsePolynomialBitsFeedBack(polynomial) : Util::GetInfoBitsFeedBackFromPolynomial(polynomial);
		ErrorChain e;
		BaseError* typeError = &e;

		for (size_t i = 0; i < 2; ++i)
		{
			for (size_t j = 0; j < positionBitsFeedback.size(); ++j)
			{
				Period::BOOLVALUE value = Period::BOOLVALUE::FALSE;
				if (i != 0)
				{
					value = Period::BOOLVALUE::TRUE;
				}
				Period::CalcPeriod(registerStr, positionBitsFeedback, value, j, typeError);
			}
		}
		std::cout << "\n";
	}
}

namespace TestPeriodErrorAdder
{
	void test(const std::string& registerStr, std::string& polynomial, bool flag)
	{
		auto positionBitsFeedback = flag == true ? Util::ParsePolynomialBitsFeedBack(polynomial) : Util::GetInfoBitsFeedBackFromPolynomial(polynomial);
		ErrorAdder e;
		BaseError* typeError = &e;

		for (size_t i = 0; i < 2; ++i)
		{
			for (size_t j = 0; j < positionBitsFeedback.size() - 1; ++j)
			{
				Period::BOOLVALUE value = Period::BOOLVALUE::FALSE;
				if (i != 0)
				{
					value = Period::BOOLVALUE::TRUE;
				}
				Period::CalcPeriod(registerStr, positionBitsFeedback, value, j, typeError);
			}
		}
		std::cout << "\n";
	}
}
