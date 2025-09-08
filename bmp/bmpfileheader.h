#ifndef BMPFILEHEADER_H
#define BMPFILEHEADER_H

#include <cstdint>

class BmpFileHeader
{
public:
    BmpFileHeader();
    BmpFileHeader(int NZwidth, int height);

    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

#endif // BMPFILEHEADER_H
