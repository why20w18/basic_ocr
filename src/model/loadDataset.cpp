#include "../../include/model/loadDataset.hpp"


LoadDataset::LoadDataset(const std::string &folderPath,std::vector<std::vector<double>> &inputDataRaw,
                         std::vector<std::vector<double>> &oneHotLabels,bool normalized,bool thresholdRead ,int outputClassCount) 
:folderPath(folderPath), outputClassCount(outputClassCount)
{

    for(const auto &file : std::filesystem::directory_iterator(folderPath)){
/////////(1)EGITIM SETINDEN VERIYI OKUMA////////////
        //dosyanin yolu cikartildi
        std::string filePath = file.path().string();
        
        //dosyanin uzantisini kontrol etme
        //test.bmp
        if(filePath.substr(filePath.length()-1-3) == ".bmp"){
            //ham bmp dosyası okundu ve 2D arraye eklendi
            bmpReader reader(filePath);
            auto rawPixels = reader.readConvert(normalized);
            
            if(thresholdRead){
                ThresholdOp threshold(THRESHOLD_DATASET_USER_PARAMS,normalized);
                threshold.initalize(rawPixels,reader.getWidth(),reader.getHeight());
                threshold.applyPointOperation();
                rawPixels = threshold.getRawPixelData();
            }
            

            inputDataRaw.push_back(rawPixels);

/////////(2)ONE HOT LABEL DOLDURMA ISLEMI////////////
            //etiket cikartma islemi
            std::vector<double> labels(outputClassCount,0.0f);

            std::size_t pos = filePath.find_last_of("/\\"); //filename
            std::string fileName = (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
            
            //ilk karakter harf degilse etiket cikartilamaz
            if (fileName.size() > 0 && std::isalpha(fileName[0])) {
                char ch = std::tolower(fileName[0]); 
                int index = ch - 'a'; 
            
                if (index >= 0 && index < 26) {
                    labels[index] = 1.0f;
                } else {
                    std::cerr << "[HATA] GECERSIZ ETIKET " << ch << "\n";
                }
            } else {
                std::cerr << "[HATA] ETIKETSIZ DOSYA ADI " << fileName << "\n";
            }

            oneHotLabels.push_back(labels);

        }
    }    
}