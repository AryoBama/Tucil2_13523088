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
            if (channels == 4) result[y][x].setAlpha(data[index+3]);

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

   
    unsigned char* imageData = new unsigned char[width * height * channels];

    int idx = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            
            imageData[idx++] = static_cast<unsigned char>(std::min(255, std::max(0, image[i][j].getRed())));
            imageData[idx++] = static_cast<unsigned char>(std::min(255, std::max(0, image[i][j].getGreen())));
            imageData[idx++] = static_cast<unsigned char>(std::min(255, std::max(0, image[i][j].getBlue())));
            if (channels == 4) idx++;
        }
    }

    return imageData;
}

void image_write_func(void* context, void* data, int size) {
    vector<unsigned char>* buf = reinterpret_cast<vector<unsigned char>*>(context);
    unsigned char* byteData = reinterpret_cast<unsigned char*>(data);
    buf->insert(buf->end(), byteData, byteData + size);
}

unsigned char* imageByCompressionRate(const vector<vector<Color>>& image, double compressionRate, int errorMethod, uintmax_t sizeBytes, 
    int &nNode, int &depthTree, int channels){

    int maxThreshold = 0, minThreshold = 0, maxRange = 0, maxActualThreshold = 0;
    double tolerance = 0;
    compressionRate *= 100;
    double percentage = 0;
    int minBlock = 2;

    unsigned char* imageData = nullptr;

    int height = image.size(), width = image[0].size();

    if (errorMethod == 1){
        maxRange = maxThreshold = 650000;
        tolerance = 5;
        maxActualThreshold = 650000;
    }else if (errorMethod == 2){
        maxRange =maxThreshold = 100000;
        tolerance = 5;
        maxActualThreshold = 127;
    }else if (errorMethod == 3){
        maxRange =maxThreshold = 100000;
        tolerance = 5;
        maxActualThreshold = 255;
    }else if (errorMethod == 4){
        maxRange =maxThreshold = 100000;
        tolerance = 5;
        maxActualThreshold = 8;
    }else if (errorMethod == 5){
        maxRange = maxThreshold = 100000;
        tolerance = 5;
        maxActualThreshold = 1;
    }

    do{

        
        if (imageData != nullptr) {
            delete[] imageData;
        }
        
        int mid = (maxThreshold + minThreshold)/2;

        double actualThreshold = ((double)mid / maxRange) * maxActualThreshold;

        Quadtree quadtree(image,errorMethod,actualThreshold,minBlock,compressionRate/100);

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
        depthTree = quadtree.countDepth(quadtree.getRoot(),-1);

        int loading = percentage/compressionRate * 100;

        cout << "Loading... " << loading << "%\n";
        



    }while(maxThreshold - minThreshold  >= 2 && abs(percentage - compressionRate) > tolerance);


    return imageData;

}

void createGif(const vector<std::vector<Color>> &image, unsigned char* compressImageData, const char* filename, int width, int height, int depthTree, int channels) {
    GifWriter writer;
    GifBegin(&writer, filename, width, height, 10);
    int minBlock = width * height;

    int loading = 0;

    cout << "Membuat GIF...\n";
    cout << "Loading... " << loading << " %\n";

    
    uint8_t trans_r = 0, trans_g = 0, trans_b = 0;

    for (int i = 0; i < max(1, depthTree - 1); i++) {
        loading = (i + 1) * 10;
        cout << "Loading... " << loading << " %\n";

        vector<vector<Color>> frame(height, vector<Color>(width));
        vector<uint8_t> frameData(width * height * 4);

        Quadtree quadtree(image, 6, 0, minBlock, 0);
        mergeBlock(quadtree.getRoot(), frame, width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& c = frame[y][x];
                int index = (y * width + x) * 4;

                uint8_t alpha = (channels == 4) ? image[y][x].getAlpha() : 255;

                if (alpha < 128) {
                    frameData[index + 0] = trans_r;
                    frameData[index + 1] = trans_g;
                    frameData[index + 2] = trans_b;
                } else {
                    frameData[index + 0] = c.getRed();
                    frameData[index + 1] = c.getGreen();
                    frameData[index + 2] = c.getBlue();
                }

                frameData[index + 3] = 255;
            }
        }

        minBlock /= 4;
        GifWriteFrame(&writer, frameData.data(), width, height, 100);
    }


    vector<uint8_t> frameData(width * height * 4);
    for (int i = 0, j = 0; i < width * height * channels; i += channels, j += 4) {
        uint8_t r = compressImageData[i + 0];
        uint8_t g = compressImageData[i + 1];
        uint8_t b = compressImageData[i + 2];
        uint8_t a = (channels == 4) ? compressImageData[i + 3] : 255;

        if (a < 128) {
            frameData[j + 0] = trans_r;
            frameData[j + 1] = trans_g;
            frameData[j + 2] = trans_b;
        } else {
            frameData[j + 0] = r;
            frameData[j + 1] = g;
            frameData[j + 2] = b;
        }
        frameData[j + 3] = 255;
    }

    cout << "Complete... " << 100 << " %\n";
    GifWriteFrame(&writer, frameData.data(), width, height, 100);

    GifEnd(&writer);
}

string getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of(".");
    if (dotPos != std::string::npos && dotPos != filePath.length() - 1) {
        return filePath.substr(dotPos + 1);
    }
    return "";
}