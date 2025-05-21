#include "../include/debugOCR.hpp"
#include "../include/modelProp.hpp"

#include "../include/model/neuralNetwork.hpp"
#include "../include/model/loadDataset.hpp"
#include "../include/model/segment.hpp"

#include "../include/opPoint/bmpReader.hpp"
#include "../include/opPoint/brightnessOperation.hpp"
#include "../include/opPoint/thresholdOperation.hpp"

void printResult(const char *arr);

int main(){
////////////////////////////////////////////////
    //std::cout << "->" << RandomMath::getRandom(-1,1) << "\n";
    //NeuralNetwork ann;

    /*
    bmpReader bmpFile("dataset_ab/a_001.bmp");
    auto img = bmpFile.readConvert();
    std::cout << img.size() << std::endl;
    std::cout << bmpFile.getWidth() << "<--width height--> " << bmpFile.getHeight() << std::endl;
    */

    /*
    BrightnessOp bop(120);
    bop.initalize(img,bmpFile.getWidth(),bmpFile.getHeight());
    bop.applyPointOperation();
    bop.printPixelData();
    */

    /*
    ThresholdOp top(120);
    top.initalize(img,bmpFile.getWidth(),bmpFile.getHeight());
    top.applyPointOperation();
    top.printPixelData();
    */
////////////////////////////////////////////////
    NeuralNetwork ann(LEARNING_RATE);

    std::vector<std::vector<double>> inputDataRaw;//(200,std::vector<double>(ann.getInputLayerSize()));
    std::vector<std::vector<double>> oneHotLabels;//(200,std::vector<double>(ann.getOutputLayerSize()));

    //LoadDataset loader(DATASET_AB_POSXY,inputDataRaw,oneHotLabels,ann.getOutputLayerSize());
    //std::string imgPath = DATASET_AB_POSXY"/b_013.bmp";
    
    LoadDataset loader(DATASETS"/dataset_posxy_az_en",inputDataRaw,oneHotLabels,NORMALIZE_READ,THRESHOLD_READ,OUTPUT_LAYER);
    //LoadDataset loader(DATASETS"/dataset_2_font",inputDataRaw,oneHotLabels,NORMALIZE_READ,THRESHOLD_READ,OUTPUT_LAYER);
    
    std::string imgPath = DATASETS AZ_EN"/h_011.bmp";


    DL("inputData SIZE: " << inputDataRaw.size());
    DL("oneHotLabels SIZE: " << oneHotLabels.size());
    DL("output SIZE: " << ann.getOutputs().size());

    /*
    for(const auto& r : oneHotLabels){
        std::cout << r[0] << " -- ";
        std::cout << r[1] << "\n";
    }
    */

    //getchar();

    //EGITIM
    ann.fit(inputDataRaw,oneHotLabels,TOTAL_EPOCH_TRAIN);
    //ann.loadModel(SAVE_PATH);

    //TAHMIN DEGERLERI
    ann.printPredictTest(inputDataRaw,oneHotLabels,10);


    //(1)GORUNTUYU OKU ISLE VE TAHMIN ETTIR

    //goruntuyu vektore aktarma
   
    bmpReader testFile(imgPath);
    std::vector<double> rawImage = testFile.readConvert(NORMALIZE_READ);

    //image pre process
    BrightnessOp bop(0,NORMALIZE_OP);
    bop.initalize(rawImage, testFile.getWidth(), testFile.getHeight());
    bop.applyPointOperation();
    ThresholdOp top(128,NORMALIZE_OP);
    top.initalize(rawImage, testFile.getWidth(), testFile.getHeight());
    top.applyPointOperation();

    std::vector<char> text;
    ann.predictImage(rawImage,text);
    
    for(char ch : text)
        printf("%c",ch);
    printf("\n");

    //(2)DOGRUDAN PATH VER ON ISLEME YAPMADAN TAHMIN ETTIR
    //ann.predictImage(imgPath);


    //KULLANICI METNI VERME
    std::vector<char> text_1;
    bmpReader userImageFile("/home/archw/packs/imgProcess/datasets/userImages/hello_world.bmp");
    std::vector<double> userRawImage = userImageFile.readConvert(NORMALIZE_READ);

    if(THRESHOLD_READ){
        ThresholdOp threshold(THRESHOLD_DATASET_USER_PARAMS,NORMALIZE_OP);
        threshold.initalize(userRawImage,userImageFile.getWidth(),userImageFile.getHeight());
        threshold.applyPointOperation();
        userRawImage = threshold.getRawPixelData();
    }
    
    Segment segment(userRawImage,userImageFile.getWidth(),userImageFile.getHeight());
    std::cout << "WIDTH : " << userImageFile.getWidth() << " HEIGHT : " << userImageFile.getHeight() << "\n"; 
    std::vector<std::vector<double>> segmentedChar = segment.getSegmentedChar();
    
    std::cout << "USER RAW IMAGE : " << std::endl;
    //ann.print1D(userRawImage);

    std::cout << "segment sayisi : " << segment.getCountBlock() << std::endl;
    
    int i = 0;
    for(auto oneGrid : segmentedChar){
        if(i < segment.getCountBlock()){
            ann.predictImage(oneGrid,text_1);
            i++;
        }
    }

    //metin puntosu 17 ile yazilmis olmali datasetimiz bu sekilde
    std::cout << "\nSONUC\n\n";
    char arr[11];
    short x = 0;
    for(char ch : text_1){
        arr[x] = ch;
        printf("%c",arr[x]);
        x++;
    }
    printf("\n");
    arr[x] = '\n';
    printResult(arr);
    ann.saveModel();


    return 0x0;
}

void printResult(const char *arr){
    std::cout << "HIDDEN_LAYER      :   " << HIDDEN_LAYER << std::endl; 
    std::cout << "LEARNING_RATE     :   " << LEARNING_RATE << std::endl;
    std::cout << "TOTAL_EPOCH_TRAIN :   " << TOTAL_EPOCH_TRAIN << std::endl;
    std::cout << "THRESHOLD_READ    :   " << THRESHOLD_READ << std::endl;
    std::cout << "NORMALIZE_READ    :   " << NORMALIZE_READ << std::endl;
    std::cout << "CIKMASI GEREKEN   :   " << "helloworld" << std::endl;
    std::cout << "SONUC             :   " << arr << std::endl;
}