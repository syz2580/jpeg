#ifndef _JPEG_H_
#define _JPEG_H_

#include "tool.h"
#include "CJfif.h"
#include "CRle.h"
#include "CDpcm.h"
#include "CHuffman.h"

//
const int LUMA_DC_CODES = 12;
const int LUMA_AC_CODES = 256;
const int CHROMA_DC_CODES = 12;
const int CHROMA_AC_CODES = 256;

// 
const int MAX_HUFF_SYMBOLS = 257;
const int MAX_HUFF_CODESIZE = 32;

const uint8 ZIGZAG[64] = 
{
    0, 1, 8, 16, 9, 2, 3, 10,
    17,24,32,25, 18,11,4, 5,
    12,19,26,33, 40,48,41,34,
    27,20,13,6,  7, 14,21,28,
    35,42,49,56, 57,50,43,36,
    29,22,15,23, 30,37,44,51,
    58,59,52,45, 38,31,39,46,
    53,60,61,54, 47,55,62,63
};

const short QUANT_TABLE[2][64] = 
{
    {
    16, 11, 12, 14,   12, 10, 16, 14, 
    13, 14, 18, 17,   16, 19, 24, 40,
    26, 24, 22, 22,   24, 49, 35, 37,
    29, 40, 58, 51,   61, 60, 57, 51,
    56, 55, 64, 72,   92, 78, 64, 68,
    87, 69, 55, 56,   80, 109,81, 87,
    95, 98, 103,104,  103,62, 77, 113,
    121,112,100,120,  92, 101,103,99  },
    {
    17,18,18,24,    21,24,47,26,
    26,47,99,66,    56,66,99,99,
    99,99,99,99,    99,99,99,99,
    99,99,99,99,    99,99,99,99,
    99,99,99,99,    99,99,99,99,
    99,99,99,99,    99,99,99,99,
    99,99,99,99,    99,99,99,99,
    99,99,99,99,    99,99,99,99 }
};

template<class T>
class CImage;

class CJpegCompressor
{
public:

    CJpegCompressor();

    virtual ~CJpegCompressor();

    /*
     * @brief  Compress a image(descripted in rgb pixels) using jpeg algorithm.
     * @return A Jfif
     */
    CJfif* Compress(const uint8 rgb[], int w, int h, int quality);

    void PrintInfo(void);

private:    
    // rgb image -> ycc image (3 channel)
    void ReadRGB(const uint8 rgb[], int w, int h, double y[], double cb[], double cr[]);

    // extend a channel (16-align)
    double* Extend(const double in[], int in_w, int in_h, int& out_w, int& out_h);

    // subsample a channel using format V:1/2 H:1/2
    double* Subsample(const double in[], const double luma[], int in_w, int in_h, int& out_w, int& out_h);

    // DCT a channel and return a sequence
    double* DCTZiazag(const double in[], int w, int h);

    // Quantize coeff of image after DCT 
    int* Quantize(const double in[], int w, int h, int id);//id=0 for luma

    void ComputeQuantTable(int id);

    void Clear(void);

private:    
    // here stupid implementation used, but it's simple
    CImage<double> *raw, *ext, *sub, *dct;

    CImage<int> *qnt;
    
    // DPCM and RLE
    CDpcm *dpcm[2];
    
    CRle *rle[2];

    int quality;

    int quantTable[2][64];

    // huffman
    CHuffman *dc_huff[2];

    CHuffman *ac_huff[2];
};

#endif