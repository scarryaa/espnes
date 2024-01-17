#include "pch.h"
#include "CppUnitTest.h"
#include "../espnes-cpp/include/cpu_helpers.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace nestests
{
	TEST_CLASS(nestests)
	{
	public:
		
		TEST_METHOD(AdcTest)
		{
			int op1 = 0;
			int op2 = 0;
			int carry = 0;
			int result = CPUHelpers::adc(op1, op2, carry);
			Assert::AreEqual(0, result);

			op1 = 1;
			op2 = 1;
			carry = 0;
			CPUHelpers::adc(1, 1, 0);
			result = CPUHelpers::adc(op1, op2, carry);
			Assert::AreEqual(2, result);

			op1 = 1;
			op2 = 1;
			carry = 1;
			result = CPUHelpers::adc(op1, op2, carry);
			Assert::AreEqual(3, result);
		}
	};
}
