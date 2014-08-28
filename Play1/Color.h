#ifndef _COLOR_H
#define _COLOR_H

struct Color {
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;

    Color() : r(0), g(0), b(0), a(0) {
    }
};

#endif _COLOR_H
