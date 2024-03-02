#include "test.hpp"



int main()
{
   //011001010 5 ячейка = 1;
    std::string registerStr = "011001010";
    std::string polynomial = "x9 + x6 + x4 + x3 + 1";
  
    TestPeriodNoError::test(registerStr, polynomial, false);
    TestPeriodErrorCellMemory::test(registerStr, polynomial, false);
    TestPeriodErrorChain::test(registerStr, polynomial, false);
    TestPeriodErrorAdder::test(registerStr, polynomial, false);
   
    return 0;
}

