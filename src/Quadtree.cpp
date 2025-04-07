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

        Color normalize = normalizeColor(image);

        double redValueY = normalize.getRed();
        double greenValueY = normalize.getGreen();
        double blueValueY = normalize.getBlue();

        double meanRedX = 0;
        double meanGreenX = 0;
        double meanBlueX = 0;

        for (size_t i = 0; i < image.size(); i++){
            for (size_t j = 0; j < image[0].size(); j++){

                meanRedX += image[i][j].getRed();
                meanGreenX += image[i][j].getGreen();
                meanBlueX += image[i][j].getBlue();

            }
        }
    
        meanRedX /= size;
        meanGreenX /= size;
        meanBlueX /= size;
        
        double varianceRedX = 0, varianceGreenX = 0, varianceBlueX = 0;

        countVariance(image, varianceRedX, varianceGreenX, varianceBlueX);

        const double C1 = pow((0.01 * 255),2);
        const double C2 = pow((0.03 * 255),2);

        double SSIMRed = ((2 * redValueY * meanRedX + C1) * C2) / ( ( pow(meanRedX,2) + pow(redValueY,2) + C1 ) * ( varianceRedX + C2 ) );
        double SSIMGreen = ((2 * greenValueY * meanGreenX + C1) * C2) / ( ( pow(meanGreenX,2) + pow(greenValueY,2) + C1 ) * ( varianceGreenX + C2 ) );
        double SSIMBlue = ((2 * blueValueY * meanBlueX + C1) * C2) / ( ( pow(meanBlueX,2) + pow(blueValueY,2) + C1 ) * ( varianceBlueX + C2 ) );

        return 0.299 * SSIMRed + 0.587 * SSIMGreen + 0.114 * SSIMBlue;
    

    }else{
        return -1;
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


    bool isThreshold = errorMethod != 5 ? error < threshold : error > threshold;
   

    if (isThreshold || (height/ 2 < minBlock/(width / 2)) || (width / 2 < minBlock/(height/ 2))){

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

    return 1 + countNode(node->getTopLeft()) + countNode(node->getTopRight()) + countNode(node->getBottomLeft()) + countNode(node->getBottomRight());
}

int Quadtree::countLeaf(QuadtreeNode *node){
    {
        if (node->isLeaf){
            return 1;
        }
    
        return countLeaf(node->getTopLeft()) + countLeaf(node->getTopRight()) + countLeaf(node->getBottomLeft()) + countLeaf(node->getBottomRight());
    }
}