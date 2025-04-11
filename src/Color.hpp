#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>
using namespace std;

class Color{

    private:
        int r;
        int g;
        int b;
        int a;
        int channels;

    public:

        Color();

        Color(int r, int g, int b);

        Color(int r, int g, int b, int a);

        Color(const Color& other);

        ~Color();

        Color& operator=(const Color& other);

        int getRed() const;

        int getBlue() const;

        int getGreen() const;

        int getAlpha() const;

        int getChannels() const;

        void setRed(int r);

        void setBlue(int b);

        void setGreen(int g);

        void setAlpha(int a);

};

#endif