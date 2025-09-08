#ifndef BMPINFOHEADER_H
#define BMPINFOHEADER_H

#include <cstdint>

class BmpInfoHeader
{
public:
    BmpInfoHeader();
    BmpInfoHeader(int NZwidth, int height);

    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

#endif // BMPINFOHEADER_H
