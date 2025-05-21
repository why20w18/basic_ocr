#include "../../include/opPoint/thresholdOperation.hpp"


ThresholdOp::ThresholdOp(int threshold,bool isNormalize,int minThreshold , int maxThreshold)
:threshold(threshold) , minThreshold(minThreshold),maxThreshold(maxThreshold),isNormalize(isNormalize){
}

void ThresholdOp::applyPointOperation(){
    static int cx = 0;
    int totalPixel = width * height;

    if(cx % 1000 == 0)
        DL("THRESHOLD_OP_" << cx << " :" << width << " , " << height);

    if(!isNormalize)
        for(int i = 0 ; i < totalPixel ; i++){
            pixelData[i] = (pixelData[i] > threshold) ? maxThreshold : minThreshold;
        }
    else
        for(int i = 0 ; i < totalPixel ; i++)
            pixelData[i] = (pixelData[i] > (threshold/255)) ? (maxThreshold/255) : (minThreshold/255);

}

int ThresholdOp::getWidth(){
    return this->width;
}

int ThresholdOp::getHeight(){
    return this->height;
}

std::vector<double> ThresholdOp::getRawPixelData(){
    return this->pixelData;
}