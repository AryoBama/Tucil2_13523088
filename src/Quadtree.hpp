#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "QuadTreeNode.hpp"
#include <vector>



class Quadtree{

    private:
        int errorMethod;
        double threshold;
        int minBlock;
        int percentage;
        QuadtreeNode *root;

        vector<vector<Color>> copyBlock(const vector<vector<Color>> &block, int x, int y, int width, int height);
        
        double countError(const vector<vector<Color>>& image);
        
        Color normalizeColor(const vector<vector<Color>>& block);
        
        QuadtreeNode* construct(const vector<vector<Color>>& block, int x, int y, int width, int height);
        
    public:

        void countVariance(const vector<vector<Color>>& image, double &varianceRed, double &varianceGreen, double &varianceBlue);
        
        Quadtree(const vector<vector<Color>>& image, int errorMethod, double threshold, int minBlock, int percentage);

        QuadtreeNode* getRoot();

        int countDepth(QuadtreeNode *node, int cnt);

        int countNode(QuadtreeNode *node);

        int countLeaf(QuadtreeNode *node);
        
};


#endif