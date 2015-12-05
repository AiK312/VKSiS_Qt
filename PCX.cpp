#include "PCX.h"

PCX::PCX(std::string fileName)
{
    Load(fileName);
}

PCX::~PCX()
{
    if (header != NULL)
        delete[] header;
    if (pixels != NULL)
        delete[] pixels;
}

int PCX::Width()
{
    return frameW;
}

int PCX::Height()
{
    return frameH;
}

BYTE *PCX::Raw()
{
    return pixels;
}

void PCX::WriteRaw(BYTE *sourceS, BYTE *sourceE)
{
       std::copy(sourceS, sourceE, pixels);
}

void PCX::Load(std::string fileName)
{
    std::ifstream img(fileName, std::ios_base::binary);
    if (!img)
    {
        std::cout << "Can't open image : " << fileName << std::endl;
        //throw std::ifstream::failure("Can't open image : " + fileName);
        return;
    }

    img.seekg(0, std::ios::end);
    int fileSize = img.tellg();
    img.seekg(0, std::ios::beg);

    header = (PCXHEADER*)(new char[sizeof(PCXHEADER)]);
    if (header == NULL)
    {
        std::cout << "Can't allocate memory to PCXHEADER" << std::endl;
        return;
        //throw std::exception("Can't allocate memory to PCXHEADER");
    }

    img.read((char*)header, sizeof (PCXHEADER));
    if (header->Id != 10 || header->Format != 1)
    {
        std::cout << "Bad pcx image format" << std::endl;
        return;
        //throw std::exception("Bad pcx image format");
    }

    if (header->NumberOfPlanes != 1 || header->BitsPixelPlane != 8)
    {
        std::cout << "Pcx is not 256 grayscale image" << std::endl;
        return;
        //throw std::exception("Pcx is not 256 grayscale image");
    }

    width = header->Xmax - header->Xmin + 1;
    height = header->Ymax - header->Ymin + 1;
    frameH = height + 2;
    frameW = width + 2;
    header->PaletteInfo = 1;
    pixels = new BYTE[frameH * frameW];

    auto sourceSize = fileSize - sizeof(PCXHEADER);
    auto source = new BYTE[sourceSize];
    if (source == NULL)
    {
        std::cout << "Can't allocate memory to source pcx data" << std::endl;
        return;
        //throw std::exception("Can't allocate memory to source pcx data");
    }
    img.read((char*)source, sourceSize);
    Decode(source);
    delete[] source;
}

void PCX::Save(std::string fileName)
{
    std::ofstream img(fileName, std::ios_base::binary);
    if (!img)
        throw std::ofstream::failure("Can't open image : " + fileName);
    img.write((char*)header, sizeof (PCXHEADER));
    Encode(&img);
}

int PCX::DecodeLine(BYTE* sourceBytes, BYTE* decodedBytes, int lineLength)
{
    int sourcePos = 0, decodedPos = 0;;
    BYTE curByte, curByteCount;
    while (decodedPos < lineLength)
    {
        curByteCount = 1;
        curByte = sourceBytes[sourcePos++];
        if ((curByte & COUNTFLAG) == COUNTFLAG)
        {
            curByteCount = curByte & COUNTMASK;
            curByte = sourceBytes[sourcePos++];
        }
        for (; curByteCount && decodedPos < lineLength; --curByteCount, ++decodedPos)
            decodedBytes[decodedPos] = curByte;
    }
    return sourcePos;
}

void PCX::Decode(BYTE* source)
{
    for (int i = 0, size = 0; i < height; ++i)
        size += DecodeLine(source + size, pixels + width * i, width);
}

int PCX::EncodeLine(BYTE* sourceBytes, BYTE* encodedBytes, int lineLength)
{
    int sourcePos = 0, encodedPos = 0;
    BYTE curByte, curByteCount;
    while (sourcePos < lineLength)
    {
        for (curByteCount = 1, curByte = sourceBytes[sourcePos]; curByte == sourceBytes[sourcePos + curByteCount] && sourcePos + curByteCount < lineLength && curByteCount < COUNTMASK; ++curByteCount);

        if (curByte < COUNTFLAG && curByteCount == 1)
            encodedBytes[encodedPos++] = curByte;
        else
        {
            encodedBytes[encodedPos++] = curByteCount | COUNTFLAG;
            encodedBytes[encodedPos++] = curByte;
        }
        sourcePos += curByteCount;
    }
    return encodedPos;
}

void PCX::Encode(std::ofstream *img)
{
    auto buf = new BYTE[width * 2];
    for (int i = 0, size = 0; i < height; i++)
    {
        size = EncodeLine(pixels + width * i, buf, width);
        img->write((char*)buf, size);
    }
    delete[] buf;
}
