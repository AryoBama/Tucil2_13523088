#include "Color.hpp"

Color::Color() : r(0), g(0), b(0), channels(0){

}

Color::Color(int r, int g, int b) : r(r), g(g), b(b), a(0), channels(3){

}

Color::Color(int r, int g, int b, int a): r(r), g(g), b(b), a(a), channels(4){

}

Color::Color(const Color& other){
    r = other.r;
    g = other.g;
    b = other.b;
    channels = other.channels;

    if(channels == 4) a = other.a;

}

Color::~Color(){

}

Color& Color::operator=(const Color& other){

    r = other.r;
    g = other.g;
    b = other.b;
    channels = other.channels;

    if(channels == 4) a = other.a;
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

int Color::getAlpha() const{
    return a;
}

int Color::getChannels() const{
    return channels;
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

void Color::setAlpha(int a){
    this->a = a;
}