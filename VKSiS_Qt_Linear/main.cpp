#include <QCoreApplication>
#include <QTime>
#include <iostream>
#include <conio.h>
#include "../lib/ImgUtils.hpp"
#include "../lib/pcx.hpp"

inline void FilterRaw(int count, PCX* image, unsigned char* newPixels, int imageSize)
{
    for(int i=0; i<count; i++)
    {
        ImgUtils::Filter(image->Raw(), newPixels, image->Height(), image->Width());
        image->WriteRaw(newPixels, newPixels+imageSize);
    }
}

int main()
{
    QTime begin;
    begin.start();
    std::cout << "Linear pcx-image processing program" << std::endl;
    QTime timer;
    timer.start();
    PCX* image = new PCX("D:\\Image\\NoiseInp.pcx");
    std::cout << "Open file time: " << timer.elapsed() << "ms" << std::endl;
    int imageSize = image->Height() * image->Width();
    auto newPixels = new unsigned char[imageSize];

    timer.start();
    FilterRaw(5, image, newPixels, imageSize);
    //ImgUtils::MedianFilter(image->Raw(), newPixels, image->Height(), image->Width());
    std::cout << "Filter time: " << timer.elapsed() << " ms" << std::endl;

    timer.start();
    ImgUtils::Diff_Oper_III(image->Raw(), newPixels, image->Height(), image->Width());
    std::cout << "Diff Oper III time: " << timer.elapsed() << " ms" << std::endl;
    image->WriteRaw(newPixels, newPixels+imageSize);

    timer.start();
    image->Save("D:\\ResultLinear.pcx");
    std::cout << "Save time: " << timer.elapsed() << " ms" << std::endl;
    std::cout << "Program time: " << begin.elapsed() << " ms" << std::endl;
    _getch();
    delete[] newPixels;
    return 0;
}


