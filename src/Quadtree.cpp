#include "Quadtree.hpp"
#include <math.h>

vector<vector<Color>> Quadtree:: copyBlock(const vector<vector<Color>> &block, int x, int y, int width, int height){

    vector<vector<Color>> result(height, vector<Color>(width));
    
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            result[i][j] = block[y + i][x + j];
        }
    }
    return result;

}

void Quadtree::countVariance(const vector<vector<Color>>& image, double &varianceRed, double &varianceGreen, double &varianceBlue){
    
    double meanRed = 0;
    double meanGreen = 0;
    double meanBlue = 0;
    varianceRed = varianceGreen = varianceBlue = 0;

    double size = image.size() * image[0].size();

            
    for (size_t i = 0; i < image.size(); i++){
        for (size_t j = 0; j < image[0].size(); j++){
            meanRed += image[i][j].getRed();
            meanGreen += image[i][j].getGreen();
            meanBlue += image[i][j].getBlue();
        }
    }

    meanRed /= size;
    meanGreen /= size;
    meanBlue /= size;


    for (size_t i = 0; i < image.size(); i++){
        for (size_t j = 0; j < image[0].size(); j++){

            varianceRed += pow(image[i][j].getRed() - meanRed, 2);
            varianceGreen += pow(image[i][j].getGreen() - meanGreen, 2);
            varianceBlue += pow(image[i][j].getBlue() - meanBlue, 2);

        }
    }
    varianceRed /= size;
    varianceGreen /= size;
    varianceBlue /= size;

}

double Quadtree::countError(const vector<vector<Color>>& image){

    double size = image.size() * image[0].size();

    if(errorMethod == 1){

        double varianceRed = 0, varianceGreen = 0, varianceBlue = 0;

        countVariance(image, varianceRed, varianceGreen, varianceBlue);

        return (varianceRed + varianceGreen + varianceBlue)/3.0;

    }else if (errorMethod == 2){

            
            double meanRed = 0;
            double meanGreen = 0;
            double meanBlue = 0;

            
            for (size_t i = 0; i < image.size(); i++){
                for (size_t j = 0; j < image[0].size(); j++){

                    meanRed += image[i][j].getRed();
                    meanGreen += image[i][j].getGreen();
                    meanBlue += image[i][j].getBlue();

                }
            }

            meanRed /= size;
            meanGreen /= size;
            meanBlue /= size;

            double MADRed = 0, MADGreen = 0, MADBlue = 0;

            for (size_t i = 0; i < image.size(); i++){
                for (size_t j = 0; j < image[0].size(); j++){

                    MADRed += abs(image[i][j].getRed() - meanRed);
                    MADGreen += abs(image[i][j].getGreen() - meanGreen);
                    MADBlue += abs(image[i][j].getBlue() - meanBlue);

                }
            }

            MADRed /= size;
            MADGreen /= size;
            MADBlue /= size;

            return (MADRed + MADGreen + MADBlue)/3.0;

    }else if (errorMethod == 3){
        
            double maxRed = -1, minRed = 256;
            double maxGreen = -1, minGreen = 256;
            double maxBlue = -1, minBlue = 256;

            for (size_t i = 0; i < image.size(); i++){
                for (size_t j = 0; j < image[0].size(); j++){

                    double red = image[i][j].getRed();
                    double green = image[i][j].getGreen();
                    double blue = image[i][j].getBlue();

                    if (red  > maxRed){
                        maxRed = red;
                    }

                    if (red < minRed){
                        minRed = red;
                    }

                    if (green > maxGreen){
                        maxGreen = green;
                    }

                    if (green < minGreen){
                        minGreen = green;
                    }

                    if (blue > maxBlue){
                        maxBlue = blue;
                    }

                    if (blue < minBlue){
                        minBlue = blue;
                    }

                }
            }

            double DRed = maxRed - minRed;
            double DGreen = maxGreen - minGreen;
            double DBlue = maxBlue - minBlue;

            return (DRed + DGreen + DBlue)/3.0;

    }else if (errorMethod == 4){
        
        int freqTableRed[256];
        int freqTableGreen[256];
        int freqTableBlue[256];

        for (int i = 0 ; i < 256; i++){
            freqTableRed[i] = 0;
            freqTableGreen[i] = 0;
            freqTableBlue[i] = 0;
        }

        for (size_t i = 0; i < image.size(); i++){
            for (size_t j = 0; j < image[0].size(); j++){

                freqTableRed[image[i][j].getRed()]++;
                freqTableGreen[image[i][j].getGreen()]++;
                freqTableBlue[image[i][j].getBlue()]++;

            }
        }

        double entropyRed = 0;
        double entropyGreen = 0;
        double entropyBlue = 0;

        for (int i = 0; i < 256; i++) {

            if (freqTableRed[i] > 0) {
                double p = (double)freqTableRed[i] / size;
                entropyRed += p * log2(p);
            }
            if (freqTableGreen[i] > 0) {
                double p = (double) freqTableGreen[i]/ size;
                entropyGreen += p * log2(p);
            }
            if (freqTableBlue[i] > 0) {
                double p = (double)freqTableBlue[i]/ size;
                entropyBlue += p * log2(p);
            }

        }

        entropyRed *= -1;
        entropyGreen *= -1;
        entropyBlue *= -1;
        
        return (entropyRed + entropyGreen + entropyBlue) / 3.0;
        
    }else if (errorMethod == 5){

        
        double varianceRedX = 0, varianceGreenX = 0, varianceBlueX = 0;

        countVariance(image, varianceRedX, varianceGreenX, varianceBlueX);

        const double C2 = pow((0.03 * 255),2);

        double SSIMRed = (C2) / (varianceRedX + C2);
        double SSIMGreen = (C2) /  (varianceGreenX + C2);
        double SSIMBlue = (C2) / (varianceBlueX + C2  );

        return 0.299 * SSIMRed + 0.587 * SSIMGreen + 0.114 * SSIMBlue;
    

    }else{
        return 1;
    }
}

Color Quadtree:: normalizeColor(const vector<vector<Color>>& block){

    int meanR = 0;
    int meanG = 0;
    int meanB = 0;
    int size = block.size() * block[0].size();

    for (size_t i = 0; i < block.size() ; i++){
        for (size_t j = 0; j < block[0].size(); j++){

            meanR += block[i][j].getRed();
            meanG += block[i][j].getGreen();
            meanB += block[i][j].getBlue();

        }
    }
    meanR /= size;
    meanG /= size;
    meanB /= size;
    

    return Color(meanR, meanG, meanB);

}

QuadtreeNode* Quadtree:: construct(const vector<vector<Color>>& block, int x, int y, int width, int height){

    // if (countError(block)<0){
    //     cout << "DD" << endl;
    // }
    
    double error = countError(block);
    // cout << error << endl;


    bool isHomogeneus = errorMethod != 5 ? error < threshold : error > threshold;
   

    if (isHomogeneus || (height * width / 4 < minBlock)){

        Color normalize = normalizeColor(block);

        return new QuadtreeNode(normalize, x, y, width, height);
    }


    int w1 = width / 2;
    int w2 = width - w1;
    int h1 = height / 2;
    int h2 = height - h1;

    return new QuadtreeNode(
        construct(copyBlock(block, 0, 0, w1, h1), 
        x, y, w1, h1),
        construct(copyBlock(block, w1, 0, w2, h1), 
                x + w1, y, w2, h1),

        construct(copyBlock(block, 0, h1, w1, h2), 
                x, y + h1, w1, h2),

        construct(copyBlock(block, w1, h1, w2, h2), 
                x + w1, y + h1, w2, h2),
        x, y, width, height
    );

}

Quadtree::Quadtree(const vector<vector<Color>>& image, int errorMethod, double threshold, int minBlock, int percentage)
    : errorMethod(errorMethod), threshold(threshold), minBlock(minBlock), percentage(percentage){

    root = construct(image,0,0,image[0].size(), image.size());

}

QuadtreeNode* Quadtree::getRoot(){
    return root;
}

int Quadtree::countDepth(QuadtreeNode *node, int cnt){

    if (node->isLeaf){
        return cnt;
    }

    int topLeft = countDepth(node->getTopLeft(), cnt + 1);
    int topRight = countDepth(node->getTopRight(), cnt + 1);
    int bottomLeft = countDepth(node->getBottomLeft(), cnt + 1);
    int bottomRight = countDepth(node->getBottomRight(), cnt + 1);

    return max(max(topLeft, topRight), max(bottomLeft, bottomRight));

}

int Quadtree::countNode(QuadtreeNode *node){

    if (node->isLeaf){
        return 1;
    }
    if (node != root){

        return 1 + countNode(node->getTopLeft()) + countNode(node->getTopRight()) + countNode(node->getBottomLeft()) + countNode(node->getBottomRight());
    }
    return countNode(node->getTopLeft()) + countNode(node->getTopRight()) + countNode(node->getBottomLeft()) + countNode(node->getBottomRight());
}

int Quadtree::countLeaf(QuadtreeNode *node){
    {
        if (node->isLeaf){
            return 1;
        }
    
        return countLeaf(node->getTopLeft()) + countLeaf(node->getTopRight()) + countLeaf(node->getBottomLeft()) + countLeaf(node->getBottomRight());
    }
}