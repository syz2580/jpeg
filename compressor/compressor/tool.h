#ifndef _TOOL_H_
#define _TOOL_H_

void ASSERT_PRINT(const char* file, int line);
#define ASSERT(test) do{ if (!(test)) ASSERT_PRINT(__FILE__, __LINE__); }while(0)

typedef unsigned char uint8;

inline int max(int a, int b) 
{
    return (a > b) ? a : b;
}

inline int min(int a, int b) 
{
    return (a > b) ? b : a;
}

// read a BMP file, return pixels
uint8* ReadBMP(const char* fileName, int& w, int& h);

// r g b -> y cb cr
void RGB2YCC(uint8 r, uint8 g, uint8 b, double& y, double& cb, double& cr);

// DCT a 8x8 matrix
void DCT(double data[]);

//to do
void IDCT(double data[]);

// count the numbits of 't'
int CountBits(int t);

#endif