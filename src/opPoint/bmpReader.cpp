#include "../../include/opPoint/bmpReader.hpp"

static int cx = 0;

bmpReader::bmpReader(const std::string &filePath){
    file.open(filePath,std::ios::binary);
    if(!file.is_open()){
        DL("DOSYA ACILAMADI !");
    }
    else{
        if(cx % 1000 == 0)
            DL("DOSYA BASARIYLA ACILDI ! : NO_" << cx+1);
        cx++;
    }
}

std::vector<double> bmpReader::readConvert(bool normalized){
    //header okuma
    unsigned char header[TOTAL_HEADER];
    file.read(reinterpret_cast<char*>(header),TOTAL_HEADER);

    //okunan header icerisinden goruntu yuksekligi ve genisligini cekme
    this->width = *(int*)&header[WIDTH_START_BYTE];  //18
    this->height = *((int*)&header[HEIGHT_START_BYTE]);    

    //colorTable atlama
    file.ignore(COLOR_TABLE);

    //asil pixel verisi
    this->pixelData = std::vector<double>();
    this->pixelData.reserve(width*height);

    for(int row = height-1; row >= 0; --row){   //son satir
        for(int col = 0; col < width; ++col){   //ilk sutun
            unsigned char pixel;                //seklinde okuyarak bmp ters olarak tutuyor vectore atma
            file.read(reinterpret_cast<char*>(&pixel),1);
            if(!normalized)
                this->pixelData.emplace_back(static_cast<double>(pixel));
            else
                this->pixelData.emplace_back(static_cast<double>(pixel) / 255.0);
        }
    }
    
    file.close();
    return this->pixelData;
}

int bmpReader::getWidth(){
    return this->width;
}

int bmpReader::getHeight(){
    return this->height;
}