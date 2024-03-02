#pragma once
#include <sstream>
#include <vector>

namespace Util
{
    std::string SubString(const std::string& string, int beginIndex, int endIndex)
    {
        int size = (int)string.size();
        if (beginIndex < 0 || beginIndex > size - 1)
            return "-1"; // Index out of bounds
        if (endIndex < 0 || endIndex > size - 1)
            return "-1"; // Index out of bounds
        if (beginIndex > endIndex)
            return "-1"; // Begin index should not be bigger that end.

        std::string substr;
        for (int i = 0; i < size; i++)
            if (i >= beginIndex && i < endIndex)
                substr += (char)string[i];
        return substr;
    }

    void OverwriteRegister(std::vector<uint8_t>& seqRegister, const uint8_t newBit)
    {
        for (int i = seqRegister.size() - 1; i > 0; i--)
        {
            seqRegister[i] = seqRegister[i - 1];
        }
        seqRegister[0] = newBit;
    }


    void EraseSpaceStr(std::string& str)
    {
        std::string copy(str);
        str = "";
        for (size_t i = 0; i < copy.size(); ++i)
        {
            if (!isspace(copy[i]))
            {
                str += copy[i];
            }
        }
    }

    std::vector<uint8_t> GetInfoBitsFeedBackFromCoefficientsPolynomial(std::vector<uint8_t>& coefficientsPolynomial)
    {
        std::vector<uint8_t> infoBitsFeedBack;
        for (size_t i = 1; i < coefficientsPolynomial.size(); ++i)
        {
            if (coefficientsPolynomial[i] == 1)
            {

                int num = (1 << coefficientsPolynomial.size() - 1) - (1 << (coefficientsPolynomial.size() - 1 - i));
                uint8_t countBitsOne = 0;
                while (num)
                {
                    if (num & 1)
                    {
                        countBitsOne++;
                    }
                    num >>= 1;
                }
                infoBitsFeedBack.push_back(countBitsOne);
            }
        }
        return infoBitsFeedBack;
    }

    std::vector<uint8_t> ParsePolynomialBitsFeedBack(std::string& polynomial)
    {
        std::vector<uint8_t> coefficientsPolynomial;
        EraseSpaceStr(polynomial);
        std::string sizeRegister = SubString(polynomial, polynomial.find('x') + 1, polynomial.find('+'));
        int sizeVector = std::stoi(sizeRegister) + 1;

        coefficientsPolynomial.resize(sizeVector);

        std::istringstream iss(polynomial);
        std::string term;

        while (std::getline(iss, term, '+'))
        {
            size_t found = term.find('x');
            if (found != std::string::npos)
            {
                std::string power = "";
                if (found + 1 == term.size() - 1)
                {
                    power += term[found + 1];
                }
                else
                {
                    power = term.substr(found + 1, term.size() - found - 1);
                }
                coefficientsPolynomial[coefficientsPolynomial.size() - std::stoi(power) - 1] = 1;
            }
            else
            {
                coefficientsPolynomial[coefficientsPolynomial.size() - 1] = 1;
            }
        }
        return GetInfoBitsFeedBackFromCoefficientsPolynomial(coefficientsPolynomial);
    }

   

    std::vector<uint8_t> GetInfoBitsFeedBackFromPolynomial(std::string& polynomial)
    {
        std::vector<uint8_t> positionBitsFeeadBack;
        EraseSpaceStr(polynomial);
       
        std::istringstream iss(polynomial);
        std::string term;
        while (std::getline(iss, term, '+'))
        {
            size_t found = term.find('x');
            if (found != std::string::npos)
            {
                std::string power = "";
                if (found + 1 == term.size() - 1)
                {
                    power += term[found + 1];
                }
                else
                {
                    power = term.substr(found + 1, term.size() - found - 1);
                }
                positionBitsFeeadBack.push_back(std::stoi(power));
            }
        }
        return positionBitsFeeadBack;
    }

    std::vector<uint8_t> ParseRegisterStr(const std::string& registerStr)
    {
        std::vector<uint8_t> registerBits;
        registerBits.resize(registerStr.size());
        for (size_t i = 0; i < registerStr.size(); ++i)
        {
            if (registerStr[i] == '1')
            {
                registerBits[i] = 1;
            }
        }
        return registerBits;
    }

    int CalcNumber(std::vector<uint8_t> &  registerBits)
    {
        //std::cout << "Register " << std::endl;
        int n = 0;
        for (int i = 0; i < registerBits.size(); ++i)
        {
            n = (n << 1) | registerBits[i];
         // std::cout << (int)registerBits[i];
        }
       // std::cout << std::endl;
        return n;
    }

    uint8_t CalcBitFeedBack(std::vector<uint8_t>& registerBits, std::vector<uint8_t>& positionBitsFeedback)
    {
        uint8_t newBit = 0;
        for (size_t i = 0; i < positionBitsFeedback.size(); ++i)
        {
            newBit ^= registerBits[positionBitsFeedback[i] - 1];
        }
        return newBit;
    }
}