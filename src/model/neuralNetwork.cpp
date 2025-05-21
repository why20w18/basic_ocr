#include "../../include/model/neuralNetwork.hpp"

#include <omp.h>

NeuralNetwork::NeuralNetwork(double learningRate){
    omp_set_num_threads(THREAD_COUNT);
    this->learningRate = learningRate;

    //(1) veri yapilarini dogru boyutlarda baslatma 2D arrayleri kurma
    input.resize(this->inputLayerSize);
    hidden.resize(this->hiddenLayerSize);
    output.resize(this->outputLayerSize);

    weightInput_Hidden.resize(inputLayerSize,std::vector<double>(hiddenLayerSize));
    weightHidden_Output.resize(hiddenLayerSize,std::vector<double>(outputLayerSize));

    biasHidden.resize(this->hiddenLayerSize);
    biasOutput.resize(this->outputLayerSize);

    //(2) bias degerlerini random -1 ve 1 arasinda baslatma
    for(int i = 0 ; i < hiddenLayerSize ; i++)
        biasHidden[i] = RandomMath::getRandom(-1,1);

    for(int i = 0 ; i < outputLayerSize ; i++)
        biasOutput[i] = RandomMath::getRandom(-1,1);

    
    //(3) agirlik matrislerini random sekilde baslatma
    for(int i = 0 ; i < inputLayerSize ; i++)
        for(int j = 0 ; j < hiddenLayerSize ; j++)
            weightInput_Hidden[i][j] = RandomMath::getRandom();

    for(int i = 0 ; i < hiddenLayerSize ; i++)
        for(int j = 0 ; j < outputLayerSize ; j++)
            weightHidden_Output[i][j] = RandomMath::getRandom();
    DL("NEURAL NETWORK AGIRLIKLARI VE BIASLARI RANDOM SEKILDE BASLATILDI");
    DL("INPUT  LAYER SIZE : " << inputLayerSize);
    DL("HIDDEN LAYER SIZE : " << hiddenLayerSize);
    DL("OUTPUT LAYER SIZE : " << outputLayerSize);
    
}

void NeuralNetwork::forwardPropagation(const std::vector<double> &inputData){
    //(1)INPUT LAYERDAN HIDDEN LAYERA GECIS ==> kisacasi SIGMOID(input_Nx x hidden_Nx) = hidden olarak tuttuk , kartezyen yapildi
    //input -> hidden
    input = inputData;

    #pragma omp parallel for
    for(int i = 0 ; i < hiddenLayerSize ; i++){
        hidden[i] = biasHidden[i]; //bias degerini direkt kopyaladik
        for(int j = 0 ; j < inputLayerSize ; j++){
            hidden[i] += weightInput_Hidden[j][i] * input[j]; //w*x + bias = y 
        }
        hidden[i] = RandomMath::sigmoidExp(hidden[i]); //sigmoid ciktisi olarak degistirilir 
    }

    //(2)HIDDEN LAYERDAN OUTPUT LAYERA GECIS ==> 1 output seciliyor tum hiddenler icin hesaplama yapilip sigmoidden geciriliyor
    //hidden -> output
    #pragma omp parallel for
    for(int i = 0 ; i < outputLayerSize ; i++){
        output[i] = biasOutput[i];
        for(int j = 0 ; j < hiddenLayerSize ; j++){
            output[i] += weightHidden_Output[j][i] * hidden[j];
        }
        output[i] = RandomMath::sigmoidExp(output[i]);
    }
}

void NeuralNetwork::backwardPropagation(std::vector<double> &outputData,std::vector<double> &hiddenData,const std::vector<double> &target){
    //(1)OUTPUT->HIDDEN
    std::vector<double> errorOutput(outputLayerSize,0.);
    #pragma omp parallel for 
    for(int i = 0 ; i < outputLayerSize ; i++){
        errorOutput[i] = (target[i] - outputData[i]) * RandomMath::derOut_sigmoidExp(outputData[i]);
    }
    //errorOutput dolduruldu hidden layerdaki hatalari hesaplamak icin geri yayacagiz

    //(2)HIDDEN LAYER ERROR
    //hidden layerdaki 1 noron output layerdaki 1'den fazla noronla hesaplama yapiyor bu sebeple
    /*
        h1          o1      h1_o1_err + h1_o2_err = h1_total_err
        h2          o2      h2_o1_err + h2_o2_err = h2_total_err
        ...         ...
    */
    std::vector<double> errorHidden(hiddenLayerSize,0.);
    #pragma omp parallel for collapse(2)
    for(int i = 0 ; i < hiddenLayerSize ; i++){
        double hi_total_err = 0.0f;
        for(int j = 0 ; j < outputLayerSize ; j++){
            hi_total_err += errorOutput[j] * weightHidden_Output[i][j];
        }
        errorHidden[i] = hi_total_err * RandomMath::derOut_sigmoidExp(hiddenData[i]);
    }

    //learning burada gerceklesiyor esasinda eski weightsleri silmeden guncelleme:
    //(3)HIDDEN->OUTPUT WEIGHTS guncelleme
    #pragma omp parallel for collapse(2)
    for(int i = 0 ; i < hiddenLayerSize ; i++)
        for(int j = 0 ; j < outputLayerSize ; j++){
            weightHidden_Output[i][j] += this->learningRate * errorOutput[j] * hiddenData[i]; 
        }

    //(4)INPUT->HIDDEN WEIGHTS guncelleme
    #pragma omp parallel for collapse(2)
    for(int i = 0 ; i < inputLayerSize ; i++){
        for(int j = 0 ; j < hiddenLayerSize ; j++){ //tum hidden layer noronlari ile isleniyor 1 input noronu
            weightInput_Hidden[i][j] += this->learningRate * errorHidden[j] * input[i];
        }
    }
    
    //eski bias degerlerini silmeden guncelleme:
    //(4)OUTPUT BIAS
    #pragma omp parallel for 
    for(int i = 0 ; i < outputLayerSize ; i++)
        biasOutput[i] += this->learningRate * errorOutput[i];

    //(5)HIDDEN BIAS
    #pragma omp parallel for
    for(int i = 0 ; i < hiddenLayerSize ; i++){
        biasHidden[i] += this->learningRate * errorHidden[i];
    }

    
}

void NeuralNetwork::train(const std::vector<double> &inputData , const std::vector<double> &target){
    forwardPropagation(inputData);
    backwardPropagation(output,hidden,target);
}

int NeuralNetwork::getInputLayerSize(){
    return this->inputLayerSize;
}

int NeuralNetwork::getHiddenLayerSize(){
    return this->hiddenLayerSize;
}

int NeuralNetwork::getOutputLayerSize(){
    return this->outputLayerSize;
}

double NeuralNetwork::toRound(double outputValue){
    if(outputValue < 1e-4) return 0;
    else return outputValue;
}


void NeuralNetwork::printPredictTest(const std::vector<std::vector<double>> &inputDataRaw
                               ,const std::vector<std::vector<double>> &oneHotLabels,
                               int loopCount
                            ){
    
    int correctCount = 0;
    for(int i = 0; i < loopCount; i++){
        forwardPropagation(inputDataRaw[i]);
        
        int predictedIndex = std::distance(output.begin(),std::max_element(output.begin(),output.end()));
        int actualIndex = std::distance(oneHotLabels[i].begin(), std::max_element(oneHotLabels[i].begin(), oneHotLabels[i].end()));
    
        //tahminlerin ciktisi
        printf("TAHMIN [%5d] | ",(i+1));
        for(double val : this->output){
            printf("(%3.4lf) ",val);
        }

        //one hot label => olmasi gereken
        std::cout << " | GERCEK DEGER : ";
        for(int j = 0 ; j < outputLayerSize ; j++)
            printf("(%2.0lf) ",oneHotLabels[i][j]);

        if(predictedIndex == actualIndex) {
            std::cout << " TAM TAHMIN";
            correctCount++;
        }
        else
            std::cout << " HATALI TAHMIN";
            std::cout << "\n";

    }

    double accuracy = 100.0 * correctCount / loopCount;
    std::cout << "\nORAN: " << correctCount << "/" << loopCount
              << " | DOGRULUK ORANI: %" << accuracy << std::endl;
    

}

std::vector<double>& NeuralNetwork::getOutputs(){
    return this->output;
}

void NeuralNetwork::predictImage(const std::string &pathfile,bool normalized,std::vector<char> &text){
    DL("TAHMIN EDILECEK DOSYA : " << pathfile);
    bmpReader reader(pathfile);
    auto v = reader.readConvert(normalized);
    forwardPropagation(v);

    int predictIndex = std::distance(                   //indexi en buyuk olanin 
        output.begin(),                                 //cikti vektorunun ilk iteratoru
        std::max_element(output.begin(),output.end())   //cikti vektorunun icindeki en buyuk iterator
    );

    predictIndexSwitch(predictIndex,text);
}

void NeuralNetwork::predictImage(std::vector<double> rawImageData,std::vector<char> &text){
    forwardPropagation(rawImageData);

    int predictIndex = std::distance(                   //indexi en buyuk olanin 
        output.begin(),                                 //cikti vektorunun ilk iteratoru
        std::max_element(output.begin(),output.end())   //cikti vektorunun icindeki en buyuk iterator
    );

   predictIndexSwitch(predictIndex,text);
}

void NeuralNetwork::predictIndexSwitch(int predictIndex,std::vector<char> &text){
    if (predictIndex >= 0 && predictIndex <= 25) {
        char predictedChar = 'a' + predictIndex;
        text.push_back(predictedChar);
        std::cout << predictedChar << " ";
    } else {
        std::cerr << "KARAKTER TESPIT BASARISIZ!\n";
    }
    
}


void NeuralNetwork::fillRange(std::vector<int> &sortedVec,int startValue,int increment){
    for(int i = 0 ; i < sortedVec.size() ; i ++){
        sortedVec[i] = startValue + increment -1; 
        increment++;
    }
}

void NeuralNetwork::fit(const std::vector<std::vector<double>> &inputDataRaw,
                        const std::vector<std::vector<double>> &oneHotLabels,int totalEpoch){
    for(int epoch = 0; epoch < totalEpoch; ++epoch){
        if(epoch % PRINT_COUNT_FIT == 0)
            std::cout << "\nEPOCH: " << epoch+1 << "/" << totalEpoch << std::flush;

    
        std::vector<int> indices(inputDataRaw.size());
        fillRange(indices,0);

        std::shuffle(indices.begin(),indices.end(),RandomMath::getGenerator());
    
        for (int randomIndex : indices) {
            train(inputDataRaw[randomIndex], oneHotLabels[randomIndex]);
        }

        //this->learningRate *= 0.99;
    }
}

void NeuralNetwork::printWeights(){
    for(auto &r : this->weightInput_Hidden){
        for(int i = 0 ; i < r.size() ; i++){
            std::cout << r[i] << " ";
        }
    }
    
    for(auto &r : this->weightHidden_Output){
        for(int i = 0 ; i < r.size() ; i++){
            std::cout << r[i] << " ";
        }
    }
}

void NeuralNetwork::saveModel(){
    std::string path = SAVE_PATH;
    std::ofstream out(path, std::ios::binary);
    if(!out.is_open()){
        std::cerr << "MODEL KAYIT ACILAMADI ! : " << path << std::endl;
        return;
    }

    out.write(reinterpret_cast<char*>(&inputLayerSize), sizeof(inputLayerSize));
    out.write(reinterpret_cast<char*>(&hiddenLayerSize), sizeof(hiddenLayerSize));
    out.write(reinterpret_cast<char*>(&outputLayerSize), sizeof(outputLayerSize));

    for(auto& row : weightInput_Hidden)
        out.write(reinterpret_cast<char*>(row.data()), row.size() * sizeof(double));

    for(auto& row : weightHidden_Output)
        out.write(reinterpret_cast<char*>(row.data()), row.size() * sizeof(double));

    out.write(reinterpret_cast<char*>(biasHidden.data()), biasHidden.size() * sizeof(double));
    out.write(reinterpret_cast<char*>(biasOutput.data()), biasOutput.size() * sizeof(double));

    out.close();
    std::cout << "MODEL KAYDEDILDI ! : " << path << std::endl;
}

void NeuralNetwork::loadModel(const std::string &path){
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "MODEL DOSYASI BULUNAMADI ! : " << path << std::endl;
        return;
    }

    in.read(reinterpret_cast<char*>(&inputLayerSize), sizeof(inputLayerSize));
    in.read(reinterpret_cast<char*>(&hiddenLayerSize), sizeof(hiddenLayerSize));
    in.read(reinterpret_cast<char*>(&outputLayerSize), sizeof(outputLayerSize));

    input.resize(inputLayerSize);
    hidden.resize(hiddenLayerSize);
    output.resize(outputLayerSize);

    weightInput_Hidden.resize(inputLayerSize, std::vector<double>(hiddenLayerSize));
    weightHidden_Output.resize(hiddenLayerSize, std::vector<double>(outputLayerSize));

    biasHidden.resize(hiddenLayerSize);
    biasOutput.resize(outputLayerSize);

    for (int i = 0; i < inputLayerSize; ++i)
        in.read(reinterpret_cast<char*>(weightInput_Hidden[i].data()), hiddenLayerSize * sizeof(double));

    for (int i = 0; i < hiddenLayerSize; ++i)
        in.read(reinterpret_cast<char*>(weightHidden_Output[i].data()), outputLayerSize * sizeof(double));

    in.read(reinterpret_cast<char*>(biasHidden.data()), hiddenLayerSize * sizeof(double));
    in.read(reinterpret_cast<char*>(biasOutput.data()), outputLayerSize * sizeof(double));

    in.close();
    std::cout << "MODEL YÜKLENDİ ! : " << path << std::endl;
}
