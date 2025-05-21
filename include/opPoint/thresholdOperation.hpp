#ifndef __THRESHOLD_OPERATION_HPP__
#define __THRESHOLD_OPERATION_HPP__

#include "pointOperation.hpp"

class ThresholdOp : public PointOperation{
private:
    int minThreshold;
    int maxThreshold;
    int threshold;
    bool isNormalize;

public:
    ThresholdOp(int threshold,bool isNormalize,int minThreshold = 0 , int maxThreshold = 255);

    int getWidth() override;
    int getHeight() override;
    std::vector<double> getRawPixelData() override;
    void applyPointOperation() override;            
};


#endif // __THRESHOLD_OPERATION_HPP__