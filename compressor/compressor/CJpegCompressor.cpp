#include "CJpegCompressor.h"
#include "CImage.h"
#include "tool.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

CJpegCompressor::CJpegCompressor()
    :raw(0),
    ext(0),
    sub(0),
    dct(0),
    qnt(0),
    quality(100)
{
    dpcm[0] = 0;
    dpcm[1] = 0;
    rle[0] = 0;
    rle[1] = 0;
    dc_huff[0] = 0;
    dc_huff[1] = 0;
    ac_huff[0] = 0;
    ac_huff[1] = 0;
}

CJpegCompressor::~CJpegCompressor()
{
    Clear();
}

CJfif* CJpegCompressor::Compress(const uint8 rgb[], int w, int h, int quality)
{
    this->quality = quality;

    // rgb->ycc
    raw = new CImage<double>(w, h);
    ASSERT(0 != raw);
    ReadRGB(rgb, w, h, raw->y, raw->cb, raw->cr);

    // extend
    ext = new CImage<double>();
    ext->y  = Extend(raw->y,  raw->yw, raw->yh, ext->yw, ext->yh);
    ext->cb = Extend(raw->cb, raw->cw, raw->ch, ext->cw, ext->ch);
    ext->cr = Extend(raw->cr, raw->cw, raw->ch, ext->cw, ext->ch);

    // subsample
    sub = new CImage<double>();
    sub->y = new double[ext->yw * ext->yh];
    memcpy(sub->y, ext->y, sizeof(double) * ext->yw * ext->yh);
    sub->yw = ext->yw;
    sub->yh = ext->yh;
    sub->cb = Subsample(ext->cb, ext->y, ext->cw, ext->ch, sub->cw, sub->ch);
    sub->cr = Subsample(ext->cr, ext->y, ext->cw, ext->ch, sub->cw, sub->ch);
        
    // DCT & zigzag
    dct = new CImage<double>();
    dct->y  = DCTZiazag(sub->y,  sub->yw, sub->yh);
    dct->cb = DCTZiazag(sub->cb, sub->cw, sub->ch);
    dct->cr = DCTZiazag(sub->cr, sub->cw, sub->ch);
    dct->yw = sub->yw;
    dct->yh = sub->yh;
    dct->cw = sub->cw;
    dct->ch = sub->ch;

    // Quantize
    ComputeQuantTable(0);
    ComputeQuantTable(1);

    qnt = new CImage<int>();
    qnt->y = Quantize(dct->y, dct->yw, dct->yh, 0);
    qnt->cb= Quantize(dct->cb,dct->cw, dct->ch, 1);
    qnt->cr= Quantize(dct->cr,dct->cw, dct->ch, 1);
    qnt->yw = dct->yw;
    qnt->yh = dct->yh;
    qnt->cw = dct->cw;
    qnt->ch = dct->ch;
    
    // DPCM    
    int size_data = qnt->yh * qnt->yw / 64;
    int *data = new int[size_data];
    ASSERT(0 != data);
    for (int i=0; i<size_data; i++)
    {
        data[i] = qnt->y[64*i];
    }
    dpcm[0] = new CDpcm;
    ASSERT(0 != dpcm[0]);
    dpcm[0]->SetData(data, size_data);
    dpcm[0]->Compute();
    delete[] data;

    size_data = qnt->ch * qnt->cw / 64 * 2;// 2 channels
    data = new int[size_data];
    ASSERT(0 != data);
    for (int i=0; i<size_data / 2; i++)
    {
        data[i] = qnt->cb[64*i];
        data[i + size_data / 2] = qnt->cr[64*i];
    }
    dpcm[1] = new CDpcm;
    ASSERT(0 != dpcm[1]);
    dpcm[1]->SetData(data, size_data);
    dpcm[1]->Compute();
    delete[] data;

    // RLE
    size_data = qnt->yw * qnt->yh - (qnt->yw * qnt->yh / 64);
    data = new int[size_data];    
    int count = 0;
    for (int i=0; i < (qnt->yw * qnt->yh / 64); i++)
    {
        for (int j=1; j<64; j++)
        {
            data[count++] = qnt->y[i*64+j];
        }
    }
    rle[0] = new CRle;
    ASSERT(0 != rle[0]);
    rle[0]->SetData(data, size_data);
    rle[0]->Compute();
    delete[] data;
    
    size_data = 2 * (qnt->cw * qnt->ch - (qnt->cw * qnt->ch / 64));// 2 channels
    data = new int[size_data];
    count = 0;
    for (int i=0; i < (qnt->cw * qnt->ch / 64); i++)    //for every block(64)
    {
        for (int j=1; j<64; j++)
        {
            data[count] = qnt->cb[i*64+j];
            data[size_data/2 + count] = qnt->cr[i*64+j];
            count++;
        }
    }
    rle[1] = new CRle;
    ASSERT(0 != rle[1]);
    rle[1]->SetData(data, size_data);
    rle[1]->Compute();
    delete[] data;

    // huff
    int size = dpcm[0]->GetSize();
    const int* t = dpcm[0]->GetResult();
    int16 *t16 = new int16[size];
    ASSERT(0 != t16);
    for (int i=0; i<size; i++)
    {
        t16[i] = static_cast<int16>(t[i]);
    }
    dc_huff[0] = new CHuffman(size, t16);
    dc_huff[0]->huffman_encode("temp.txt");
    int16* depth = dc_huff[0]->get_depth();
    ASSERT(0 != depth);
    delete t16;
    /*for (int i=0; i<2; i++)
    {
        for (int j=0; j < dpcm[i]->size_data; j++)
        {
            dc_huff[i].count[CountBits(dpcm[i]->result[j])]++;
        }
        dc_huff[i].Compute();
    }
    for (int i=0; i<2; i++)
    {
        for (int j=0; j < rle[i]->size_data; j++)
        {
            ac_huff[i].count[(CountBits(rle[i]->num_zero[j] << 4) + CountBits(rle[i]->var[j]))] += 1;
        }
        ac_huff[i].Compute();
    }*/

    // jfif
    /*CJfif *jfif = new CJfif;
    ASSERT(0 != jfif);
    int size_ydc_nbits = dpcm[0]->GetSize();
    int 
    uint8 *ydc_nbits = new uint8[size_ydc_nbits];
    for (int i=0; i<size_ydc_nbits; i++)
    {
        ydc_nbits[i] = CountBits(dpcm[i]->result[i]);
    }*/

    //jfif->SetDQT();
    //jfif->SetSize(w, h);
    //jfif->SetDHT();
    //jfif->SetData();

    return 0;
}

void CJpegCompressor::PrintInfo(void)
{
    cout << "@raw: " << endl;
    raw->Print(0);
    cout << "@ext: " << endl;
    ext->Print(0);
    cout << "@sub: " << endl;
    sub->Print(0);
    cout << "@dct: " << endl;
    dct->Print(8);
    cout << "@qnt: " << endl;
    qnt->Print(8);
    cout << "@RLE:(y ac) " << endl;
    rle[0]->Print();    
    cout << "@RLE:(c ac) " << endl;
    rle[1]->Print();    
    cout << "@DPCM:(y dc) " << endl;
    dpcm[0]->Print();   
    cout << "@DPCM:(c dc) " << endl;
    dpcm[1]->Print();
}

void CJpegCompressor::ReadRGB(const uint8* rgb, int w, int h, double y[], double cb[], double cr[])
{
    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j++)
        {
            RGB2YCC(rgb[(i*w+j)*3], rgb[(i*w+j)*3+1], rgb[(i*w+j)*3+2], y[i*w+j], cb[i*w+j], cr[i*w+j]);
        }
    }
}

double* CJpegCompressor::Extend(const double in[], int in_w, int in_h, int& out_w, int& out_h)
{
    // extended w and h
    out_w = 16 * ((in_w -1) / 16 + 1);
    out_h = 16 * ((in_h -1) / 16 + 1);
    double* out = new double[out_w * out_h];
    ASSERT(0!=out);

    for (int i=0; i<in_h; i++)
    {
        for (int j=0; j<in_w; j++)
        {
            out[i*out_w+j] = in[i*in_w+j];
        }
        // extend cols
        for (int j=in_w; j<out_w; j++)
        {
            out[i*out_w+j] = out[i*out_w+in_w-1];
        }
    }
    // extend rows
    for (int i=in_h; i<out_h; i++)
    {
        for (int j=0; j<out_w; j++)
        {
            out[i*out_w+j] = out[(in_h-1)*out_w+j];
        }
    }
    return out;
}

double* CJpegCompressor::Subsample(const double in[], const double luma[], int in_w, int in_h, int& out_w, int& out_h)
{
    out_w = in_w / 2;
    out_h = in_h / 2;
    double* out = new double[out_w*out_h];
    ASSERT(0!=out);

    for (int i=0; i<out_h; i++)
    {
        for (int j=0; j<out_w; j++)
        {
            double a = 129-std::abs(luma[2*i*in_w+2*j]);
            double b = 129-std::abs(luma[2*i*in_w+2*j+1]);
            double c = 129-std::abs(luma[(2*i+1)*in_w+2*j]);
            double d = 129-std::abs(luma[(2*i+1)*in_w+2*j+1]);
            out[i*out_w + j] = (in[2*i*in_w+2*j]*a + in[2*i*in_w+2*j+1]*b
                + in[(2*i+1)*in_w+2*j]*c + in[(2*i+1)*in_w+2*j+1]*d)/(a+b+c+d);
        }
    }
    return out;
}

double* CJpegCompressor::DCTZiazag(const double in[], int w, int h)
{
    double* out = new double[w*h];
    ASSERT(0!=out);
    memset(out, 0, sizeof(double)*w*h);

    int col = w/8;
    int row = h/8;
    for (int i=0; i<row; i++)
    {
        for (int j=0; j<col; j++)
        {
            for (int m=0; m<8; m++)
            {
                for (int n=0; n<8; n++)
                {
                    out[64*(i*col+j)+m*8+n] = in[(i*8+m)*w + j*8+n];
                }
            }
            //!
            DCT(&out[64*(i*col+j)]);

            //zigzag
            double t[64];
            memcpy(t, &out[64*(i*col+j)], sizeof(double)*64);
            for (int k=0; k<64; k++)
            {
                out[64*(i*col+j) + k] = t[ZIGZAG[k]];
            }
        }
    }
    return out;
}

int* CJpegCompressor::Quantize(const double in[], int w, int h, int id)
{
    int* out = new int[w*h];
    ASSERT(0 != out);

    for (int i=0; i<w*h/64; i++)
    {
        for (int j=0; j<64; j++)
        {
            double t = in[i*64 + j];
            int quant = quantTable[id][j];
            if (t < 0) 
            {
                int tmp = static_cast<int>(-t + (quant >> 1));
                out[i*64 + j] =  (tmp < quant) ? 0 : static_cast<int>(-(tmp / quant));
            } 
            else 
            {
                int tmp = static_cast<int>(t + (quant >> 1));
                out[i*64 + j] = (tmp < quant) ? 0 : static_cast<int>((tmp / quant));
            }
        }
    }
    return out;
}

void CJpegCompressor::ComputeQuantTable(int id)
{
    double q;
    if (quality < 50) 
    {
        q = 5000.0 / quality;
    } 
    else 
    {
        q = 200.0 - quality * 2.0;
    }
    for (int i=0; i<64; i++) 
    {
        int t = QUANT_TABLE[id][i];
        t = static_cast<int>((t * q + 50L) / 100L);
        quantTable[id][i] = min(max(t, 1), 1024/3);
    }
    // DC quantized worse than 8 makes overall quality fall off the cliff
    if (quantTable[id][0] > 8) 
    {
        quantTable[id][0] = (quantTable[id][0]+8*3)/4;
    }
    if (quantTable[id][1] > 24)
    {
        quantTable[id][1] = (quantTable[id][1]+24)/2;
    }
    if (quantTable[id][2] > 24) 
    {
        quantTable[id][2] = (quantTable[id][2]+24)/2;
    }
}

void CJpegCompressor::Clear(void)
{
    if (0 != raw)
    {
        delete raw;
        raw = 0;
    }
    if (0 != ext)
    {
        delete ext;
        ext = 0;
    }
    if (0 != sub)
    {
        delete sub;
        sub = 0;
    }
    if (0 != dct)
    {
        delete dct;
        dct = 0;
    }
    if (0 != qnt)
    {
        delete qnt;
        qnt = 0;
    }
    for (int i=0; i<2; i++)
    {
        if (0 != dpcm[i])
        {
            delete dpcm[i];
            dpcm[i] = 0;
        }
        if (0 != rle[i])
        {
            delete rle[i];
            rle[i] = 0;
        }
        if (0 != dc_huff[i])
        {
            delete dc_huff[i];
            dc_huff[i] = 0;
        }
        if (0 != ac_huff[i])
        {
            delete ac_huff[i];
            ac_huff[i] = 0;
        }
    }
}


