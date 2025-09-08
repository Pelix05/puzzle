#include "bmpinfoheader.h"

BmpInfoHeader::BmpInfoHeader()
{

}

BmpInfoHeader::BmpInfoHeader(int NZwidth, int height)
{
    int width = (NZwidth + 3) / 4 * 4;
    biSize = 40;
    biWidth = (int32_t) width;
    biHeight = (int32_t) height;
    biPlanes = 1;
    biBitCount = 24;
    biCompression = 0;
    biSizeImage = (uint32_t) (width * height * 3);
    biXPelsPerMeter = 0;
    biYPelsPerMeter = 0;
    biClrUsed = 0;
    biClrImportant = 0;
}

