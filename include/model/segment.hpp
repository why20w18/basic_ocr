#ifndef __SEGMENT_HPP__
#define __SEGMENT_HPP__

#include "../modelProp.hpp"
#include "../opPoint/bmpReader.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

class Segment{
private:
    std::vector<std::vector<double>> segmentedChar;
    std::vector<double> rawImage;
    int imageWidth;
    int imageHeight;
    int gridSize;

    int countBlock;

    void segmentImage();

public:
    Segment(std::vector<double> &rawImage,int imageWidth,int imageHeight,int gridSize = GRID_SIZE);
    std::vector<std::vector<double>> getSegmentedChar();
    int getCountBlock();

    //project
    std::pair<int, int> findVerticalBounds(); //yatay projeksiyon
    std::vector<std::pair<int, int>> findCharacterBounds(int top, int bottom); //dikey projeksiyon
    std::vector<std::vector<double>> to2D(const std::vector<double> &flat, int width, int height);
    std::vector<std::vector<double>> resizeTo28x28(const std::vector<std::vector<double>> &input, int inW, int inH);
    std::vector<double> flatten(const std::vector<std::vector<double>> &mat);


};


#endif //__SEGMENT_HPP__
/*
segmentasyon:
    -28x28'lik izgaralara bolsem
    -her 28x28 izgarayi tarayip icinde siyah var mi kontrol etsem
    -eger siyah yoksa direkt o izgarayi atlasam
    -siyah olan her izgarayida noral aga gonderip cikti alsam
    -aldigim ciktilarida birlestirip kullaniciya gostersem

    SONUC: grid metodmus ise yaramadi
    -yatay dikey projeksiyon ile segmentleme basarili oldu
*/