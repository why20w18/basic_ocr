#ifndef __BMP_READER_HPP__
#define __BMP_READER_HPP__

#include "pointOperation.hpp"



#define TOTAL_HEADER 54
#define HEADER 14
#define INFO_HEADER 40
#define COLOR_TABLE 1024 //256 adet renk * 4byte
#define WIDTH_START_BYTE 18
#define HEIGHT_START_BYTE 22


class bmpReader : public PointOperation{
private:
   
    std::ifstream file;
public:
    bmpReader(const std::string &filePath);
    std::vector<double> readConvert(bool normalized);
    
    int getWidth() override;
    int getHeight() override;

};

#endif //__BMP_READER_HPP__
/*
https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

*/