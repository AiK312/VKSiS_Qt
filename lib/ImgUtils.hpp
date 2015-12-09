#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "math.h"


class ImgUtils
{
private:

	static inline void SortWindow(unsigned char* window)
	{
		std::sort(window, window + 9);
	}

    static inline void LoadLine(unsigned char* source, unsigned char* window)
    {
        window[0] = source[-1];
        window[1] = source[0];
        window[2] = source[1];
    }

    static inline void LoadWindow(unsigned char* source, int y, int x, int width, unsigned char* window)
    {
        LoadLine(source + (y - 1) * width + x, window + 0);
        LoadLine(source + (y + 0) * width + x, window + 3);
        LoadLine(source + (y + 1) * width + x, window + 6);
    }

    static inline void FilterWindow(unsigned char* window)
    {
        unsigned char temp = 0;
        for(int i=0; i<9; i++)
            temp += window[i]/9;
        window[4] = temp;

    }

    static inline void DiffFilterWindow(unsigned char* window)
    {
        window[4] = abs(window[0]+window[1]+window[2]-window[6]-window[7]-window[8]) +
                    abs(window[0]+window[3]+window[6]-window[2]-window[5]-window[8]);

    }

public:      

    static void Diff_Oper_III(unsigned char* original, unsigned char* processed, int heigth, int width)
    {
        int hig = heigth - 1, wid = width - 1;
        auto window = new unsigned char[9];
        for(int i=1; i<hig; i++)
        {
            for(int j=1; j<wid; j++)
            {
                LoadWindow(original, i, j, width, window);
                DiffFilterWindow(window);
                processed[i*width+j] = window[4];

            }
        }
        delete[] window;
    }

    static void Filter(unsigned char* original, unsigned char* processed, int heigth, int width)
    {

        int hig = heigth - 1, wid = width - 1;
        auto window = new unsigned char[9];
        for(int i=1; i<hig; i++)
        {
            for(int j=1; j<wid; j++)
            {
                LoadWindow(original, i, j, width, window);
                FilterWindow(window);
                processed[i*width+j] = window[4];

            }
        }
        delete[] window;
    }
	
	static void MedianFilter(unsigned char* original, unsigned char* processed, int heigth, int width)
	{
		auto window = new unsigned char[9];
		int H = heigth - 1, W = width - 1;
		for (int i = 1; i < H; ++i)
			for (int j = 1; j < W; ++j)
			{
				LoadWindow(original, i, j, width, window);
				SortWindow(window);
				processed[i * width + j] = window[4];
			}
		delete[] window;
	}

	static void Rotate180(unsigned char* original, int size)
	{
		std::reverse(original, original + size);
	}
};
