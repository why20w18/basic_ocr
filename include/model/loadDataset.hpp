#ifndef __LOAD_DATASET_HPP__
#define __LOAD_DATASET_HPP__

#include <filesystem>
#include <string>
#include <vector>

#include "../modelProp.hpp"
#include "../debugOCR.hpp"
#include "../opPoint/bmpReader.hpp"
#include "../opPoint/thresholdOperation.hpp"

class LoadDataset{
private:
    int outputClassCount;
    std::string folderPath;

public:    
    LoadDataset(const std::string &folderPath,std::vector<std::vector<double>> &inputDataRaw,
        std::vector<std::vector<double>> &oneHotLabels,bool normalized,bool thresholdRead,int outputClassCount);
    
    //std::vector<std::vector<double>> getInputDataRaw();
    //std::vector<std::vector<double>> getOneHotLabels();
};


#endif //__LOAD_DATASET_HPP__
/*
https://stackoverflow.com/questions/70250329/how-to-convert-const-stdfilesystemdirectory-entry-to-string-in-c

*/