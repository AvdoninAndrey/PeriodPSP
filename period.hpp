#pragma once
#include <iostream>
#include <set>
#include "util.hpp"

namespace Period
{

    enum class BOOLVALUE : uint8_t
    {
        FALSE = 0,
        TRUE = 1
    };

    struct BaseError
    {

        virtual void DoError(std::vector<uint8_t>& registerBits, std::vector<uint8_t>& positionBitsFeedback, int positionError, Period::BOOLVALUE value) = 0;
        virtual void PrintError(int positionError, Period::BOOLVALUE value, int period) = 0;
    };
    struct ErrorMemomryCell;
    struct ErrorChain;
    struct ErrorAdder;
    struct NotError;

    void FindPeriod(std::vector<int>& sequnceNumbers, 
        std::vector<uint8_t>& registerBits, 
        std::vector<uint8_t> & positionBitsFeedback,
        BaseError * typeError,
        int positionError, BOOLVALUE value)
    {
        typeError->DoError(registerBits, positionBitsFeedback, positionError, value);
        int n = Util::CalcNumber(registerBits);
        
        sequnceNumbers.push_back(n);
        
        auto itFindElement = std::find(begin(sequnceNumbers), end(sequnceNumbers), n);
        int index = static_cast<int>(std::distance(begin(sequnceNumbers), itFindElement));
        int index2 = 0;
        for (int i = index + 1; i < sequnceNumbers.size(); ++i)
        {
            if (sequnceNumbers[i] == sequnceNumbers[index])
            {
                index2 = i;
                break;
            }
        }
        int period = index2 - index;
        typeError->PrintError(positionError, value, period);
    }



    bool Cicle(std::set<int>& uniqueNumbers, 
        std::vector<int>& sequnceNumbers, 
        std::vector<uint8_t>& registerBits, 
        std::vector<uint8_t>& positionBitsFeedback,
        int positionError, BOOLVALUE value,
        BaseError * typeError,
        int i, int& counter)
    {
        int n{};
        while (counter < i)
        {
           
            typeError->DoError(registerBits, positionBitsFeedback, positionError, value);
         
            n = Util::CalcNumber(registerBits);
            
            uniqueNumbers.insert(n);
            sequnceNumbers.push_back(n);
            ++counter;
        }

        return uniqueNumbers.size() < i ? true : false;
    }


    void CalcPeriod(const std::string & registerStr, std::vector<uint8_t> & positionBitsFeedback, BOOLVALUE boolvalue, uint16_t positionBit, BaseError * error)
    {
        std::vector<uint8_t> registerBits = Util::ParseRegisterStr(registerStr);
        int i = 4, counter = 0;
        std::set<int> uniqueNumbers;
        std::vector<int> sequnceNumbers;
      
        while (true)
        {
            if (Cicle(uniqueNumbers, sequnceNumbers, registerBits, positionBitsFeedback, positionBit, boolvalue, error, i, counter))
            {
                break;
            }
            else
            {
                i <<= 1;
            }
        }
      
        FindPeriod(sequnceNumbers, registerBits, positionBitsFeedback, error, positionBit, boolvalue);
    }

   

    struct ErrorMemomryCell : BaseError
    {
        void DoError(std::vector<uint8_t>& registerBits, std::vector<uint8_t>& positionBitsFeedback, int positionError, Period::BOOLVALUE value) override
        {
            registerBits[positionError] = static_cast<uint8_t>(value);
            uint8_t newBit = Util::CalcBitFeedBack(registerBits, positionBitsFeedback);
            Util::OverwriteRegister(registerBits, newBit);
            registerBits[positionError] = static_cast<uint8_t>(value);
        }

        void  PrintError(int positionError, Period::BOOLVALUE value, int period) override
        {
            std::cout << "Error memory cell[" << ++positionError << "]   bool value all = " << static_cast<int>(value) << "; Period = " << period << std::endl;
        }

    };

    struct ErrorChain : BaseError
    {
        void DoError(std::vector<uint8_t>& registerBits, std::vector<uint8_t>& positionBitsFeedback, int positionError, Period::BOOLVALUE value) override
        {
            uint8_t newBit = 0;
            uint8_t beginChain = positionBitsFeedback[positionError];

            for (size_t i = 0; i < positionBitsFeedback.size(); ++i)
            {
                if (i == positionError)
                {
                    newBit ^= static_cast<uint8_t>(value);
                }
                else
                {
                    newBit ^= registerBits[positionBitsFeedback[i] - 1];
                }
            }
            Util::OverwriteRegister(registerBits, newBit);
        }

        void  PrintError(int positionError, Period::BOOLVALUE value, int period) override
        {
            std::cout << "Error chain[" << positionError << "]   bool value all = " << static_cast<int>(value) << "; Period = " << period << std::endl;
        }

    };

    struct ErrorAdder : BaseError
    {
        void DoError(std::vector<uint8_t>& registerBits, std::vector<uint8_t>& positionBitsFeedback, int positionError, Period::BOOLVALUE value) override
        {
            uint8_t newBit = 0;
            std::vector<uint8_t> summators;
            summators.resize(positionBitsFeedback.size() - 1);
            for (size_t i = positionError; i < summators.size(); ++i)
            {
                if (i == positionError)
                {
                    summators[i] = static_cast<uint8_t>(value);
                }
                else if (i == 0 && i != positionError)
                {
                    summators[i] = (registerBits[positionBitsFeedback[i] - 1] ^ registerBits[positionBitsFeedback[i + 1] - 1]);
                }
                else if (i != 0 && i != positionError)
                {
       
                    summators[i] = (summators[i - 1] ^ registerBits[positionBitsFeedback[i + 1] - 1]);
                   
                }
                for (size_t i = 0; i < summators.size(); ++i)
                {
                    newBit ^= summators[i];
                }
            }
            Util::OverwriteRegister(registerBits, newBit);
        }

        void  PrintError(int positionError, Period::BOOLVALUE value, int period) override
        {
            std::cout << "Error adder[" << ++positionError << "]   bool value all = " << static_cast<int>(value) << "; Period = " << period << std::endl;
        }

    };
    struct NotError : BaseError
    {
        void DoError(std::vector<uint8_t>& registerBits, std::vector<uint8_t>& positionBitsFeedback, int positionError, Period::BOOLVALUE value) override
        {
            uint8_t newBit = Util::CalcBitFeedBack(registerBits, positionBitsFeedback);
            Util::OverwriteRegister(registerBits, newBit);
        }

        void  PrintError(int positionError, Period::BOOLVALUE value, int period) override
        {
            std::cout << "No error,  Period = " << period << std::endl;
        }
    };
}