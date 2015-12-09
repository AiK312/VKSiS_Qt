#include <QCoreApplication>
#include <QTime>
#include <iostream>
#include "../lib/ImgUtils.hpp"
#include "../lib/pcx.hpp"
#include <mpi.h>
#include <conio.h>


int main(int argc, char *argv[])
{
    QTime start;
    start.start();
    QTime timer;
    PCX* img = NULL;
    int imgSize[2] = {0};
    int &heigth = imgSize[0], &width = imgSize[1], rank, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    if (rank == 0)
    {
        std::cout << "Parallel pcx-image processing program" << std::endl;
        timer.start();
        img = new PCX("D:\\Image\\NoiseInp.pcx");
        std::cout << "Open file time: " << timer.elapsed() << " ms" << std::endl;
        width = img->Width();
        heigth = img->Height();
    }
    int imageSize = heigth*width;
    MPI_Bcast(&imgSize, 2, MPI_INT, 0, MPI_COMM_WORLD);
    int linePerProc = std::ceil((heigth - 2) / nproc);
    int *sConuts = new int[nproc], *sDist = new int[nproc];
    int tmp = heigth - 2;
    for (int i = 0; i < nproc; ++i)
    {
        sConuts[i] = (std::min(linePerProc, tmp) + 2) * width;
        sDist[i] = (i == 0) ? 0 : (sDist[i - 1] + sConuts[i - 1]) - 2 * width;
        tmp -= linePerProc;
    }
    unsigned char *recvBuf = new unsigned char[sConuts[rank]];
    int recvSize = sConuts[rank];
    char *sendBuf = (char*)((img != NULL) ? img->Raw() : NULL);
    MPI_Scatterv(sendBuf, sConuts, sDist, MPI_CHAR, recvBuf, recvSize, MPI_CHAR, 0, MPI_COMM_WORLD);
    unsigned char *newImg = new unsigned char[recvSize];

    timer.start();
    ImgUtils::Filter(recvBuf, newImg, recvSize / width, width);
    std::cout << "Filter time: " << timer.elapsed() << " ms" << std::endl;

    /*timer.start();
    ImgUtils::Diff_Oper_III(recvBuf, newImg, recvSize / width, width);    
    std::cout << "Diff Oper III time: " << timer.elapsed() << " ms" << std::endl;*/

    for (int i = 0; i < nproc; ++i)
    {
        sConuts[i] -= 2 * width;
        sDist[i] += width;
    }

    recvSize = sConuts[rank];
    //std::reverse(sConuts, sConuts + nproc);
    //std::reverse(sDist, sDist + nproc);

    MPI_Gatherv(newImg + width, recvSize, MPI_CHAR, sendBuf, sConuts, sDist, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {

        timer.start();
        img->Save("D:\\ResultParallel.pcx");
        std::cout << "Save file time: " << timer.elapsed() << " ms" << std::endl;
        std::cout << "Program time: " << start.elapsed() << " ms" << std::endl;
        delete img;
    }

    MPI_Finalize();
    delete[] recvBuf;
    delete[] newImg;
    return 0;
}
