#include "Color.hpp"

Color::Color() : r(0), g(0), b(0){

}

Color::Color(int r, int g, int b) : r(r), g(g), b(b){

}

Color::Color(const Color& other){
    r = other.r;
    g = other.g;
    b = other.b;
}

Color::~Color(){

}

Color& Color::operator=(const Color& other){

    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

int Color::getRed() const{
    return r;
}

int Color::getBlue() const{
    return b;
}

int Color::getGreen() const{
    return g;
}

void Color::setRed(int r) {
    this->r = r;
}

void Color::setBlue(int b){
    this->b = b;
}

void Color::setGreen(int g){
    this->g = g;
}