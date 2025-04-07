#include "QuadtreeNode.hpp"


QuadtreeNode::QuadtreeNode(Color color, int x, int y, int width, int height)
    : color(color), x(x), y(y), width(width), height(height), topLeft(nullptr), topRight(nullptr), bottomLeft(nullptr), bottomRight(nullptr), isLeaf(true) {}

QuadtreeNode::QuadtreeNode(QuadtreeNode *tl, QuadtreeNode *tr, QuadtreeNode *bl, QuadtreeNode *br, int x, int y, int width, int height)
    :  x(x), y(y), width(width), height(height), topLeft(tl), topRight(tr), bottomLeft(bl), bottomRight(br), isLeaf(false) {}

QuadtreeNode::~QuadtreeNode(){

    delete topLeft;
    delete topRight;
    delete bottomLeft;
    delete bottomRight;

}

QuadtreeNode* QuadtreeNode::getTopLeft(){
    return topLeft;
}

QuadtreeNode* QuadtreeNode::getTopRight(){
    return topRight;
}

QuadtreeNode* QuadtreeNode::getBottomLeft(){
    return bottomLeft;
}

QuadtreeNode* QuadtreeNode::getBottomRight(){
    return bottomRight;
}

int QuadtreeNode::getWidth(){
    return width;
}

int QuadtreeNode::getHeight(){
    return height;
}

int QuadtreeNode::getX(){
    return x;
}

int QuadtreeNode::getY(){
    return y;
}

Color QuadtreeNode::getColor(){
    return color;
}