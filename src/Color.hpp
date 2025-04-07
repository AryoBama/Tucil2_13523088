#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>
using namespace std;

class Color{

    private:
        int r;
        int g;
        int b;

    public:

        Color();

        Color(int r, int g, int b);

        Color(const Color& other);

        ~Color();

        Color& operator=(const Color& other);

        int getRed() const;

        int getBlue() const;

        int getGreen() const;

        void setRed(int r);

        void setBlue(int b);

        void setGreen(int g);


};

#endif