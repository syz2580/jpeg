#include "tool.h"
#include <windows.h>
#include <cstdio>
#include <fstream>

void ASSERT_PRINT(const char* file, int line)
{
	printf("ASSERT FAIL! file: %s. line: %d.\n", file, line);
    getchar();
}

uint8* ReadBMP(const char* fileName, int& w, int& h)
{
    //open file 
    std::ifstream fin(fileName, std::ios::binary|std::ios::in);
    ASSERT(true == fin.is_open());

    //read 2 header in BMP file
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    fin.read((char*)&fh, sizeof(fh));
    fin.read((char*)&ih, sizeof(ih));
    w = ih.biWidth;
    h = ih.biHeight;

    //read pixels from file(it's left-bottom to right-top!!)
    uint8* bgr = new uint8[3*w*h];    
    int n = 4 - (w*3)%4;
    bool align = (4 == n);//true for w*3%4==0, that is "4-align in row"
    char t[4];
    for (int i=0; i<h; i++)
    {
        fin.read((char*)&bgr[i*3*w], w*3);
        if (false == align)
        {            
            fin.read((char*)t, n);
        }
    }
    fin.close();

    //convert bgr to rgb and left-top to right-bottom(pixel sequence)
    uint8* rgb = new uint8[3*w*h];
    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j++)
        {
            rgb[(i*w+j)*3+0] = bgr[((h-i-1)*w+j)*3+2];
            rgb[(i*w+j)*3+1] = bgr[((h-i-1)*w+j)*3+1];
            rgb[(i*w+j)*3+2] = bgr[((h-i-1)*w+j)*3+0];
        }
    }
    delete[] bgr;

    return rgb;
}

void RGB2YCC(uint8 r, uint8 g, uint8 b, double& y, double& cb, double& cr)
{
    y  = r*0.299000 + g*0.587000 + b*0.114000 - 128.0;
    cb =-r*0.168736 - g*0.331264 + b*0.500000;
    cr = r*0.500000 - g*0.418688 - b*0.081312;
}

void DCT(double data[])
{
    double  z1, z2, z3, z4, z5,
            tmp0, tmp1, tmp2, tmp3, 
            tmp4, tmp5, tmp6, tmp7, 
            tmp10, tmp11, tmp12, tmp13;
    double *data_ptr = data;

    for (int c=0; c < 8; c++) 
    {
        tmp0 = data_ptr[0] + data_ptr[7];
        tmp7 = data_ptr[0] - data_ptr[7];
        tmp1 = data_ptr[1] + data_ptr[6];
        tmp6 = data_ptr[1] - data_ptr[6];
        tmp2 = data_ptr[2] + data_ptr[5];
        tmp5 = data_ptr[2] - data_ptr[5];
        tmp3 = data_ptr[3] + data_ptr[4];
        tmp4 = data_ptr[3] - data_ptr[4];
        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;
        data_ptr[0] = tmp10 + tmp11;
        data_ptr[4] = tmp10 - tmp11;
        z1 = (tmp12 + tmp13) * 0.541196100;
        data_ptr[2] = z1 + tmp13 * 0.765366865;
        data_ptr[6] = z1 + tmp12 * - 1.847759065;
        z1 = tmp4 + tmp7;
        z2 = tmp5 + tmp6;
        z3 = tmp4 + tmp6;
        z4 = tmp5 + tmp7;
        z5 = (z3 + z4) * 1.175875602;
        tmp4 *= 0.298631336;
        tmp5 *= 2.053119869;
        tmp6 *= 3.072711026;
        tmp7 *= 1.501321110;
        z1 *= -0.899976223;
        z2 *= -2.562915447;
        z3 *= -1.961570560;
        z4 *= -0.390180644;
        z3 += z5;
        z4 += z5;
        data_ptr[7] = tmp4 + z1 + z3;
        data_ptr[5] = tmp5 + z2 + z4;
        data_ptr[3] = tmp6 + z2 + z3;
        data_ptr[1] = tmp7 + z1 + z4;
        data_ptr += 8;
    }

    data_ptr = data;

    for (int c=0; c < 8; c++) {
        tmp0 = data_ptr[8*0] + data_ptr[8*7];
        tmp7 = data_ptr[8*0] - data_ptr[8*7];
        tmp1 = data_ptr[8*1] + data_ptr[8*6];
        tmp6 = data_ptr[8*1] - data_ptr[8*6];
        tmp2 = data_ptr[8*2] + data_ptr[8*5];
        tmp5 = data_ptr[8*2] - data_ptr[8*5];
        tmp3 = data_ptr[8*3] + data_ptr[8*4];
        tmp4 = data_ptr[8*3] - data_ptr[8*4];
        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;
        data_ptr[8*0] = (tmp10 + tmp11) / 8.0;
        data_ptr[8*4] = (tmp10 - tmp11) / 8.0;
        z1 = (tmp12 + tmp13) * 0.541196100;
        data_ptr[8*2] = (z1 + tmp13 * 0.765366865) / 8.0;
        data_ptr[8*6] = (z1 + tmp12 * -1.847759065) / 8.0;
        z1 = tmp4 + tmp7;
        z2 = tmp5 + tmp6;
        z3 = tmp4 + tmp6;
        z4 = tmp5 + tmp7;
        z5 = (z3 + z4) * 1.175875602;
        tmp4 *= 0.298631336;
        tmp5 *= 2.053119869;
        tmp6 *= 3.072711026;
        tmp7 *= 1.501321110;
        z1 *= -0.899976223;
        z2 *= -2.562915447;
        z3 *= -1.961570560;
        z4 *= -0.390180644;
        z3 += z5;
        z4 += z5;
        data_ptr[8*7] = (tmp4 + z1 + z3) / 8.0;
        data_ptr[8*5] = (tmp5 + z2 + z4) / 8.0;
        data_ptr[8*3] = (tmp6 + z2 + z3) / 8.0;
        data_ptr[8*1] = (tmp7 + z1 + z4) / 8.0;
        data_ptr++;
    }
}

int CountBits(int t)
{
    if (t < 0) 
    {
        t = -t;
    }

    int nbits = 0;
    while (t) 
    {
        nbits++;
        t >>= 1;
    }
    return nbits;
}

