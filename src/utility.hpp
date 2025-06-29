#ifndef UTILITY_HPP
#define UTILITY_HPP


#include "library/stb_image.h"
#include <vector>
#include "Color.hpp"
#include "QuadTree.hpp"
#include <stdint.h>


vector<vector<Color>> imageToVector (unsigned char* data, int width, int height, int channels);

void mergeBlock(QuadtreeNode* node, vector<vector<Color>>&result, int width, int height);

unsigned char* convertToByteArray(const vector<vector<Color>>& image, int& width, int& height, int& channels);

void image_write_func(void* context, void* data, int size);

unsigned char* imageByCompressionRate(const vector<vector<Color>>& image, double compressionRate, int errorMethod, uintmax_t sizeBytes, 
    int &nNode, int &depthTree, int channels);

void createGif(const vector<std::vector<Color>> &image, unsigned char* compressImageData, const char* filename, int width, int height, int depthTree, int channels);

string getFileExtension(const std::string& filePath);

#endif

