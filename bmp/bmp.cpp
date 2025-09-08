#include <cstdio>
#include <iostream>
#include <windows.h>
#include "bmp.h"

Bmp::Bmp()
{

}

Bmp::Bmp(BmpFileHeader &newBf, BmpInfoHeader &newBi, std::vector<uint8_t> img)
{
    //BmpFileHeader bf(NZwidth, height);
    //BmpInfoHeader bi(NZwidth, height);
    bf = newBf;
    bi = newBi;
    imgData = img;
}

void Bmp::setImgData(std::vector<uint8_t> newImgData)
{
    imgData.assign(newImgData.begin(), newImgData.end());
}

void Bmp::setBf(int NZwidth, int height)
{
    int width = NZwidth;
    //int width = (NZwidth + 3) / 4 * 4;
    //bf.bfSize = (uint32_t)(width * height + bf.bfOffBits);
    bf.bfType = 0x4D42;
    bf.bfSize = (uint32_t) (width * height * 3 + 54);
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = 54;
}

void Bmp::setBi(int NZwidth, int height)
{
    int width = NZwidth;
    //int width = (NZwidth + 3) / 4 * 4;
    bi.biSize = 40;
    bi.biWidth = (int32_t) width;
    bi.biHeight = (int32_t) height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = (uint32_t) (width * height * 3);
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
}

int Bmp::getWidth()
{
    return bi.biWidth;
}

int Bmp::getHeight()
{
    return bi.biHeight;
}

void Bmp::loadImg(const char *path)
{
    FILE* pFile;
    pFile = fopen(path, "rb");
    if(!pFile)
    {
        // return 0;
        std::cout << "loadImg() fopen return NULL, error=" << GetLastError() << std::endl;
        return;
    }

    // cout << sizeof(uint16_t) << endl << sizeof(uint32_t) << endl;
    fread(&bf.bfType, sizeof(uint16_t), 1, pFile);
    fread(&bf.bfSize, sizeof(uint32_t), 1, pFile);
    fread(&bf.bfReserved1, sizeof(uint16_t), 1, pFile);
    fread(&bf.bfReserved2, sizeof(uint16_t), 1, pFile);
    fread(&bf.bfOffBits, sizeof(uint32_t), 1, pFile);
    //cout << bmpFileHeaderData.bfSize << endl;
    //fread(&bmpFileHeaderData, sizeof(ClBitMapFileHeader), 1, pFile);
    //cout << bmpFileHeaderData.bfSize << endl;
    //cout << bmpFileHeaderData.bfReserved2;
    if(bf.bfType == 0x4D42)
    {
        //cout << "BM" << endl;

        fread(&bi.biSize, sizeof(uint32_t), 1, pFile);
        fread(&bi.biWidth, sizeof(int32_t), 1, pFile);
        fread(&bi.biHeight, sizeof(int32_t), 1, pFile);
        fread(&bi.biPlanes, sizeof(uint16_t), 1, pFile);
        fread(&bi.biBitCount, sizeof(uint16_t), 1, pFile);
        fread(&bi.biCompression, sizeof(uint32_t), 1, pFile);
        fread(&bi.biSizeImage, sizeof(uint32_t), 1, pFile);
        fread(&bi.biXPelsPerMeter, sizeof(int32_t), 1, pFile);
        fread(&bi.biYPelsPerMeter, sizeof(int32_t), 1, pFile);
        fread(&bi.biClrUsed, sizeof(uint32_t), 1, pFile);
        fread(&bi.biClrImportant, sizeof(uint32_t), 1, pFile);
        //fread(&bmpInfoHeaderData, sizeof(ClBitMapInfoHeader), 1, pFile);

        int channels = 0;
        if(bi.biBitCount == 24) channels = 3; // 24 bit count
        int linelength = bi.biWidth * channels;

        uint8_t pixVal;
        for(int i = 0; i < bi.biHeight; i++)
        {
            for(int j = 0; j < linelength; j++)
            {
                fread(&pixVal, sizeof(uint8_t), 1, pFile);
                imgData.push_back(pixVal);
            }
        }
    }

    fclose(pFile);
}

void Bmp::saveImg(char *path)
{
    FILE* pFile;
    pFile = fopen(path, "wb");
    if(!pFile)
    {
        //return 0;
        std::cout << "saveImg() fopen return NULL, error=" << GetLastError() << ", path = " << path << std::endl;
        return;
    }

    fwrite(&bf.bfType, sizeof(uint16_t), 1, pFile);
    fwrite(&bf.bfSize, sizeof(uint32_t), 1, pFile);
    fwrite(&bf.bfReserved1, sizeof(uint16_t), 1, pFile);
    fwrite(&bf.bfReserved2, sizeof(uint16_t), 1, pFile);
    fwrite(&bf.bfOffBits, sizeof(uint32_t), 1, pFile);

    fwrite(&bi.biSize, sizeof(uint32_t), 1, pFile);
    fwrite(&bi.biWidth, sizeof(int32_t), 1, pFile);
    fwrite(&bi.biHeight, sizeof(int32_t), 1, pFile);
    fwrite(&bi.biPlanes, sizeof(uint16_t), 1, pFile);
    fwrite(&bi.biBitCount, sizeof(uint16_t), 1, pFile);
    fwrite(&bi.biCompression, sizeof(uint32_t), 1, pFile);
    fwrite(&bi.biSizeImage, sizeof(uint32_t), 1, pFile);
    fwrite(&bi.biXPelsPerMeter, sizeof(int32_t), 1, pFile);
    fwrite(&bi.biYPelsPerMeter, sizeof(int32_t), 1, pFile);
    fwrite(&bi.biClrUsed, sizeof(uint32_t), 1, pFile);
    fwrite(&bi.biClrImportant, sizeof(uint32_t), 1, pFile);

    int channels = 0;
    if(bi.biBitCount == 24) channels = 3;
    int linelength = bi.biWidth * channels;

    uint8_t pixVal;
    auto iter = imgData.begin();
    for(int i = 0; i < bi.biHeight; i++)
    {
        for(int j = 0; j < linelength; j++)
        {
            pixVal = *iter;
            fwrite(&pixVal, sizeof(uint8_t), 1, pFile);
            iter++;
        }
    }
    fclose(pFile);
}

void Bmp::toGray()
{
    auto iter = imgData.begin();
    for(int i = 0; i < bi.biHeight; i++)
    {
        for(int j = 0; j < bi.biWidth; j++)
        {
            uint8_t rgbBlue = *iter;
            uint8_t rgbGreen = *(iter + 1);
            uint8_t rgbRed = *(iter + 2);
            int gray = (rgbRed * 30 + rgbGreen * 59 + rgbBlue * 11) / 100;
            *iter = (uint8_t)gray;
            *(iter + 1) = (uint8_t)gray;
            *(iter + 2) = (uint8_t)gray;
            iter += 3;

        }
    }
}

///*
void Bmp::cutImg(Bmp &newBmp, int leftDownX, int leftDownY, int rightUpX, int rightUpY)
{
    // if

    int cutWidth = rightUpX - leftDownX;
    //int cutHeight = leftDownY - rightUpY;
    int cutHeight = rightUpY - leftDownY;

    int channels = 3;
    int linelength = bi.biWidth * channels;
    int NZcutlinelength = cutWidth * channels;
    int cutlinelength = (NZcutlinelength + 3) / 4 * 4;
    //int cutlinelength = NZcutlinelength;

    std::vector<uint8_t> cutImgData;
    //auto iter = imgData.begin() + leftDownY * linelength;
    uint8_t pixVal;

    for(int i = 0; i < cutHeight; i++)
    {
        //iter += i * linelength;
        for(int j = 0; j < cutlinelength; j++)
        {
            //auto iterCut = iter;

            if(j < NZcutlinelength)
            {
                //pixVal = *(iter + j + leftDownX * channels);
                //iterCut ++;
                pixVal = imgData[i*linelength+leftDownX*channels+j];
            }
            else
                pixVal = 0;
            cutImgData.push_back(pixVal);
        }
    }

    newBmp.setBf(cutWidth, cutHeight);
    newBmp.setBi(cutWidth, cutHeight);
    newBmp.setImgData(cutImgData);
}

void Bmp::zoomImg(Bmp &newBmp, double scaleValue)
{
    int channels = 3;
    int afterNZWidth = (int) (scaleValue * bi.biWidth);
    int afterHeight = (int) (scaleValue * bi.biHeight);

    int afterWidth = (afterNZWidth + 3) / 4 * 4;
    int afterNZlinelength = channels * afterNZWidth;
    int afterlinelength = channels * afterWidth;

    uint8_t pixVal;
    std::vector<uint8_t> zoomImg;
    int linelength = bi.biWidth * channels;

    for(int i = 0; i < afterHeight; i++)
    {
        for(int j = 0; j < afterlinelength; j++)
        {
            if(j >= afterNZlinelength)
            {
                pixVal = 0;
                zoomImg.push_back(pixVal);
            }
            else
            {
                int pre_i = (int) (i / scaleValue);
                int pre_j = (int) (j / scaleValue);

                if(pre_i >= 0 && pre_i < bi.biHeight && pre_j >= 0 && pre_j < bi.biWidth)
                {
                    pixVal = imgData[pre_i * linelength + pre_j];
                    zoomImg.push_back(pixVal);
                }
            }

        }
    }

    newBmp.setBf(afterNZWidth, afterHeight);
    newBmp.setBi(afterNZWidth, afterHeight);
    newBmp.setImgData(zoomImg);
    /*
    bf.bfSize = afterlinelength * afterHeight + bf.bfOffBits;
    bi.biWidth = afterWidth;
    bi.biHeight = afterHeight;
    bi.biSizeImage = afterlinelength * afterHeight;

    this->setImgData(zoomImg);
    */
}
