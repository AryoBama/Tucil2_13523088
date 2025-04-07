#ifndef QUADTREENODE_HPP
#define QUADTREENODE_HPP

#include <iostream>
#include "Color.hpp"

using namespace std;

class QuadtreeNode{
    
    private:
        Color color;
        int x;
        int y;
        int width;
        int height;

        QuadtreeNode* topLeft;
        QuadtreeNode* topRight;
        QuadtreeNode* bottomLeft;
        QuadtreeNode* bottomRight;

    public:

        bool isLeaf;

        QuadtreeNode(Color color, int x, int y, int width, int height);

        QuadtreeNode(QuadtreeNode *tl, QuadtreeNode *tr, QuadtreeNode *bl, QuadtreeNode *br, int x, int y, int width, int height);

        ~QuadtreeNode();

    QuadtreeNode* getTopLeft();

    QuadtreeNode* getTopRight();

    QuadtreeNode* getBottomLeft();

    QuadtreeNode* getBottomRight();

    int getWidth();

    int getHeight();

    int getX();

    int getY();

    Color getColor();

};

#endif