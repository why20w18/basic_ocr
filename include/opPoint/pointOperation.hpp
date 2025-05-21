#ifndef __POINT_OPERATION_HPP__
#define __POINT_OPERATION_HPP__

#include "../debugOCR.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class PointOperation{
protected:
    std::vector<double> pixelData;
    int width;
    int height;            

public:
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    virtual std::vector<double> getRawPixelData(){ return pixelData;}

    //eger herhangi bir nokta operasyonu cagirildiysa nesneyi direkt bmpReader uzerinden atayabiliriz
    virtual void initalize(std::vector<double> pixelData,int width,int height){
        this->pixelData = std::move(pixelData);
        this->width = width;
        this->height = height;
    }
    
    virtual void applyPointOperation(){}
    virtual ~PointOperation() = default;


    //non abstract
    template<typename T = int>
    T scaleRange(T val , int minRange , int maxRange){
        if(val > maxRange)
            return maxRange;
        if(val < minRange)
            return minRange;
        return val;
    }

    void printPixelData(){
        for(int i = 0 ; i < width*height ; i++){
            if(i % 20 == 0)
                printf("\n");
            std::cout << this->pixelData[i] << " ";
        }
        printf("\n");
    }

};

#endif //__POINT_OPERATION_HPP__

