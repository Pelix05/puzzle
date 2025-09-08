#include "bmpfileheader.h"

BmpFileHeader::BmpFileHeader()
{

}

BmpFileHeader::BmpFileHeader(int NZwidth, int height)
{
    int width = (NZwidth + 3) / 4 * 4;
    bfType = 0x4D42;
    bfSize = (uint32_t) (width * height + 54);
    bfReserved1 = 0;
    bfReserved2 = 0;
    bfOffBits = 54;
}
