#ifndef BMP_H
#define BMP_H

#include <vector>
#include <cstdint>
#include "bmpfileheader.h"
#include "bmpinfoheader.h"

class Bmp
{
public:
    Bmp();
    Bmp(BmpFileHeader &newBf, BmpInfoHeader &newBi, std::vector<uint8_t> img);

    void setImgData(std::vector<uint8_t> newImgData);
    void setBf(int NZwidth, int height);
    void setBi(int NZwidth, int height);
    int getWidth();
    int getHeight();

    void loadImg(const char* path);
    void saveImg(char* path);
    void toGray();
    void cutImg(Bmp &newBmp, int leftdownx, int leftdowny, int rightupx, int rightupy);
    void zoomImg(Bmp &newBmp, double scaleValue);

private:
    BmpFileHeader bf;
    BmpInfoHeader bi;
    std::vector<uint8_t> imgData;
};

#endif // BMP_H
