#include "../../include/model/segment.hpp"


Segment::Segment(std::vector<double> &rawImage,int imageWidth,int imageHeight,int gridSize){
    this->rawImage = rawImage;
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->gridSize = gridSize;
    this->countBlock = 0;

    std::cout << "rawImage Size : " << rawImage.size() << std::endl;
    //segmentImage();

}

void Segment::segmentImage(){
    int totalGridRowCount = imageHeight / gridSize;  
    int totalGridColCount = imageWidth / gridSize;   
    
    countBlock = 0; 

    for(int gridCurrentRow = 0; gridCurrentRow < totalGridRowCount; gridCurrentRow++){
        for(int gridCurrentCol = 0; gridCurrentCol < totalGridColCount; gridCurrentCol++){
            std::vector<double> block;
            bool hasBlack = false;

            for(int y = 0; y < gridSize; ++y){
                for(int x = 0; x < gridSize; ++x){
                    int globalY = gridCurrentRow * gridSize + y;
                    int globalX = gridCurrentCol * gridSize + x;
                    
                    int idx = globalY * imageWidth + globalX;
                    
                    if (idx >= rawImage.size() || idx < 0) {
                        block.push_back(1.0); 
                        continue;
                    }

                    double val = rawImage[idx];
                    block.push_back(val);

                    if (val < SEGMENT_THRESHOLD)
                        hasBlack = true;
                    }
            }

            if(hasBlack){
                segmentedChar.push_back(block);
                countBlock++; 
            }
        }
    }
}


std::vector<std::vector<double>> Segment::getSegmentedChar(){
    this->segmentedChar.clear(); 
    auto [top, bottom] = findVerticalBounds();

    if (top == -1 || bottom == -1)
        return segmentedChar;

    auto bounds = findCharacterBounds(top, bottom);

    for (auto [startX, endX] : bounds) {
        std::vector<double> character;

        int charWidth = endX - startX + 1;
        int charHeight = bottom - top + 1;

        for (int y = top; y <= bottom; ++y) {
            for (int x = startX; x <= endX; ++x) {
                character.push_back(rawImage[y * imageWidth + x]);
            }
        }

 
        auto char2D = to2D(character, charWidth, charHeight);
        

        auto resized2D = resizeTo28x28(char2D, charWidth, charHeight);


        auto resizedFlat = flatten(resized2D);

        segmentedChar.push_back(resizedFlat);
    }

    countBlock = segmentedChar.size(); 
    return segmentedChar;
}




int Segment::getCountBlock(){
    return this->countBlock;
}

std::pair<int, int> Segment::findVerticalBounds() {
    int top = -1, bottom = -1;
    for (int y = 0; y < imageHeight;++y) {
        double rowSum = 0.0;
        for (int x = 0; x < imageWidth; ++x) {
            rowSum += rawImage[y * imageWidth + x];
        }
        if (rowSum < imageWidth * 0.95) { 
            if (top == -1) top = y;
            bottom = y;
        }
    }
    return {top, bottom};
}

std::vector<std::pair<int, int>> Segment::findCharacterBounds(int top, int bottom) {
    std::vector<std::pair<int, int>> bounds;
    bool inChar = false;
    int start = 0;

    for (int x = 0; x < imageWidth; ++x) {
        double colSum = 0.0;
        for (int y = top; y <= bottom; ++y) {
            colSum += rawImage[y * imageWidth + x];
        }

        if (colSum < (bottom - top + 1) * 0.95) {
            if (!inChar) {
                inChar = true;
                start = x;
            }
        } else {
            if (inChar) {
                inChar = false;
                bounds.emplace_back(start, x - 1);
            }
        }
    }

    if (inChar)
        bounds.emplace_back(start, imageWidth - 1);

    return bounds;
}

std::vector<std::vector<double>> Segment::to2D(const std::vector<double> &flat, int width, int height){
    std::vector<std::vector<double>> img2D(height, std::vector<double>(width));
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            img2D[y][x] = flat[y * width + x];
    return img2D;
}

std::vector<std::vector<double>> Segment::resizeTo28x28(const std::vector<std::vector<double>> &input, int inW, int inH) {
    const int outW = 28, outH = 28;
    std::vector<std::vector<double>> resized(outH, std::vector<double>(outW));

    for (int y = 0; y < outH; ++y) {
        for (int x = 0; x < outW; ++x) {
            int srcY = y * inH / outH;
            int srcX = x * inW / outW;
            resized[y][x] = input[srcY][srcX];
        }
    }
    return resized;
}

std::vector<double> Segment::flatten(const std::vector<std::vector<double>> &mat){
    std::vector<double> flat;
    for (const auto& row : mat)
        flat.insert(flat.end(), row.begin(), row.end());
    return flat;
}

