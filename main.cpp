#include <QCoreApplication>
#include <QTime>
#include <iostream>
#include <conio.h>
#include "pcx.h"
#include "ImgUtils.hpp"

/*inline void PrintElapsed(std::string text, ClockPoint start, ClockPoint end)
{
	std::cout << text << "\t:\t" << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << " ms" << std::endl;
}*/

int main(int argc, char *argv[])
{
    QTime begin;
    begin.start();
    std::cout << "Linear pcx-image processing program" << std::endl;
    QTime timer;
    timer.start();
    PCX* image = new PCX("D:\\Enlight.pcx");
    std::cout << "Open file: " << timer.elapsed() << "ms" << std::endl;
    int imageSize = image->Height() * image->Width();
    auto newPixels = new unsigned char[imageSize];
    timer.start();
    std::cout << imageSize << std::endl;
    //ImgUtils::MedianFilter(image->Raw(), newPixels, image->Height(), image->Width());
    //ImgUtils::Rotate180(newPixels, imageSize);
    //image->WriteRaw(newPixels, newPixels + imageSize);
    //ImgUtils::Diff_Oper_III(newPixels, image->Height(), image->Width());
    ImgUtils::AddNoise(newPixels, image->Height(), image->Width(), 2.5);

    image->Save("D:\\NewEnlight.pcx");
    std::cout << "Program time: " << begin.elapsed() << " ms" << std::endl;

    delete[] newPixels;
    return 0;
}



/*
int main()
{
	auto start = Clock::now();
	std::cout << "Linear pcx-image processing program" << std::endl;
	auto timer = Clock::now();
	auto img = new PCX("NoiseInp.pcx");
	PrintElapsed("Open file", timer, Clock::now());
	auto imgSize = img->Height() * img->Width();
	auto newPixels = new unsigned char[imgSize];    
	timer = Clock::now();

	ImgUtils::MedianFilter(img->Raw(), newPixels, img->Height(), img->Width());
	PrintElapsed("Median filter", timer, Clock::now());
	timer = Clock::now();
	ImgUtils::Rotate180(newPixels, imgSize);
	PrintElapsed("Rotate image", timer, Clock::now());
	timer = Clock::now();
	img->WriteRaw(newPixels, newPixels + imgSize);
	img->Save("NoiseOut.pcx");
	PrintElapsed("Save file", timer, Clock::now());
	PrintElapsed("Total time", start, Clock::now());
	delete[] newPixels;
	return 0;
}*/
