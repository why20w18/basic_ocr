#ifndef __OPERATION_RANDOM_MATH_HPP__
#define __OPERATION_RANDOM_MATH_HPP__

#include <cmath>
#include <random>

class RandomMath{
private:
    static std::mt19937 gen;
public:
    static double sigmoidExp(double x);
    static double der_sigmoidExp(double x);
    static double derOut_sigmoidExp(double ox);

    static double sigmoidRelu(double x);
    static double derOut_sigmoidRelu(double ox);

    

    static double getRandom(double start = -1 , double stop = 1);
    static std::mt19937 getGenerator();


};



#endif //__OPERATION_RANDOM_MATH_HPP__

/*
https://bilgisayarkavramlari.com/2008/11/02/sigmoid-fonksiyon/
https://cplusplus.com/reference/cmath/exp/
https://www.geeksforgeeks.org/derivative-of-the-sigmoid-function/
https://stackoverflow.com/questions/7560114/random-number-c-in-some-range

*/