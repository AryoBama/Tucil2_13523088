#include "utility.hpp"
#include "library/stb_image_write.h"
#include "library/gif.h"

vector<vector<Color>> imageToVector (unsigned char* data, int width, int height, int channels){
    if (data == nullptr){
        cout << "Failed to convert image" << endl;
        return {};
    }

    vector<vector<Color>> result(height, vector<Color>(width));

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            result[y][x].setRed(data[index]);
            result[y][x].setGreen(data[index+1]);
            result[y][x].setBlue(data[index+2]);
        }
    }
    return result;
}

void mergeBlock(QuadtreeNode* node, vector<vector<Color>>&result, int width, int height){


    if (width != (int)result[0].size() || height != (int) result.size()){
        cout << "Height or width doesn't match with vector" << endl;
        return;
    }

    if(node->isLeaf){
        for (int i = 0; i < node->getHeight(); i++){
            for (int j = 0; j < node->getWidth(); j++){
                result[node->getY() + i][node->getX() + j] = node->getColor();
            }
        }
    }else{

        mergeBlock(node->getTopLeft(), result, width, height);
        mergeBlock(node->getTopRight(), result, width, height);
        mergeBlock(node->getBottomLeft(), result, width, height);
        mergeBlock(node->getBottomRight(), result, width, height);

    }

}

unsigned char* convertToByteArray(const vector<vector<Color>>& image, int& width, int& height, int& channels) {
    width = image[0].size();
    height = image.size();
    channels = 3;  

   
    unsigned char* imageData = new unsigned char[width * height * channels];

    int idx = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            
            imageData[idx++] = static_cast<unsigned char>(std::min(255, std::max(0, image[i][j].getRed())));
            imageData[idx++] = static_cast<unsigned char>(std::min(255, std::max(0, image[i][j].getGreen())));
            imageData[idx++] = static_cast<unsigned char>(std::min(255, std::max(0, image[i][j].getBlue())));
        }
    }

    return imageData;
}

void image_write_func(void* context, void* data, int size) {
    vector<unsigned char>* buf = reinterpret_cast<vector<unsigned char>*>(context);
    unsigned char* byteData = reinterpret_cast<unsigned char*>(data);
    buf->insert(buf->end(), byteData, byteData + size);
}

unsigned char* imageByCompressionRate(const vector<vector<Color>>& image, double compressionRate, int errorMethod, uintmax_t sizeBytes, int &nNode, int &depthTree){

    int maxThreshold = 0, minThreshold = 0, maxRange = 0, maxActualThreshold = 0;
    double tolerance = 0;
    int channels = 3;
    compressionRate *= 100;
    double percentage = 0;

    unsigned char* imageData = nullptr;

    int height = image.size(), width = image[0].size();

    if (errorMethod == 1){
        maxRange = maxThreshold = 1000;
        tolerance = 5;
        maxActualThreshold = 1000;
    }else if (errorMethod == 2){
        maxRange =maxThreshold = 10000;
        tolerance = 5;
        maxActualThreshold = 127;
    }else if (errorMethod == 3){
        maxRange =maxThreshold = 10000;
        tolerance = 5;
        maxActualThreshold = 255;
    }else if (errorMethod == 4){
        maxRange =maxThreshold = 100000;
        tolerance = 5;
        maxActualThreshold = 8;
    }else if (errorMethod == 5){
        maxRange = maxThreshold = 1000;
        tolerance = 5;
        maxActualThreshold = 1;
    }

    do{

        
        if (imageData != nullptr) {
            delete[] imageData;
        }
        
        int mid = (maxThreshold + minThreshold)/2;

        double actualThreshold = ((double)mid / maxRange) * maxActualThreshold;

        Quadtree quadtree(image,errorMethod,actualThreshold,2,compressionRate/100);

        vector<vector<Color>> compression(height, vector<Color>(width));

        mergeBlock(quadtree.getRoot(), compression, width, height);

        imageData = convertToByteArray(compression, width, height, channels);

        vector<unsigned char> imageBuffer;
        stbi_write_jpg_to_func(image_write_func, &imageBuffer, width, height, 3, imageData, 90);

        percentage = ((double)imageBuffer.size()/sizeBytes )* 100;

        if (percentage > compressionRate){
            if (errorMethod != 5){
                minThreshold = mid; // kita naikkan thresholdnua biar kualitas makin jelek
            }else{
                maxThreshold = mid;
            }
        }else{
            if (errorMethod != 5){
                maxThreshold = mid; // kita turunka thresholdnua biar kualitas makin bagus
            }else{
                minThreshold = mid;
            }
        }

        // cout << "Percentage: " << percentage << endl;
        // cout << "abs: " << abs(percentage - compressionRate) << endl;

        nNode = quadtree.countNode(quadtree.getRoot());
        depthTree = quadtree.countDepth(quadtree.getRoot(),0);

        int loading = percentage/compressionRate * 100;

        cout << "Loading... " << loading << "%\n";


    }while(minThreshold <= maxThreshold && abs(percentage - compressionRate) > tolerance);


    return imageData;

}

void createGif(const vector<std::vector<Color>> &image, unsigned char* compressImageData, const char* filename, int width, int height, int depthTree) {
    GifWriter writer;
    GifBegin(&writer, filename, width, height, 10); 
    int minBlock = width*height;

    for (int i = 0; i < depthTree - 1; i++) {
        vector<vector<Color>> frame(height, vector<Color>(width));
        vector<uint8_t> frameData(width * height * 4);

        Quadtree quadtree(image,4,0,minBlock,0);
        mergeBlock(quadtree.getRoot(), frame, width, height);


        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& c = frame[y][x];
                int index = (y * width + x) * 4;
                frameData[index + 0] = c.getRed();
                frameData[index + 1] = c.getGreen();
                frameData[index + 2] = c.getBlue();
                frameData[index + 3] = 255;
            }
        }
        minBlock /= 4;

        GifWriteFrame(&writer, frameData.data(), width, height, 100);
    }

    vector<uint8_t> frameData(width * height * 4);
    for (int i = 0, j = 0; i < width * height * 3; i += 3, j += 4) {
        frameData[j + 0] = compressImageData[i + 0];
        frameData[j + 1] = compressImageData[i + 1]; 
        frameData[j + 2] = compressImageData[i + 2];
        frameData[j + 3] = 255;                      
    }

    GifWriteFrame(&writer, frameData.data(), width, height, 100);

    GifEnd(&writer);
}