#ifndef _CJFIF_H_
#define _CJFIF_H_

#include "tool.h"
#include <cstdio>

const unsigned short M_SOI = 0xFFD8;
const unsigned short M_APP0= 0xFFE0;
const unsigned short M_DQT = 0xFFDB;
const unsigned short M_SOF0= 0xFFC0;
const unsigned short M_DHT = 0xFFC4;
const unsigned short M_SOS = 0xFFDA;
const unsigned short M_EOI = 0xFFD9;

typedef struct _JFIF_APP
{
    unsigned short marker;  //0xFFE0
    unsigned short len;     //0x0010
    uint8 JFIF[5];          //JFIF+'\0'
    unsigned short version; //0x0101: v1.01
    uint8 density_unit;     //0x00: no unit
    unsigned short Xdensity;
    unsigned short Ydensity;
    unsigned short thumbnail;//0x0000: no thumbnail
}JFIF_APP;

typedef struct _JFIF_DQT
{
    unsigned short marker;  //0xFFDB
    unsigned short len;     //0x0043
    uint8 id;               //0x00 for luma, 0x01 for chroma
    uint8 data[64];         //!
}JFIF_DQT;

typedef struct _JFIF_SOF
{
    unsigned short marker;  //0xFFC0
    unsigned short len;     //0x0011
    uint8 precision;        //0x08
    unsigned short height;  //!
    unsigned short width;   //!
    uint8 num_comp;         //0x03
    struct _Comp
    {
    uint8 id;           //0x01, 0x02, 0x03
    uint8 sample;       //0x22, 0x11
    uint8 quant_table_id;//0x00, 0x01
    }Y, Cb, Cr;
}JFIF_SOF;

typedef struct _JFIF_DHT
{
    unsigned short marker;  //0xFFC4
    unsigned short len;     //0x0017
    uint8 id;               //0x00, 0x01 for luma dc and ac; 0x10, 0x11
    uint8 bits[16];         //! count when depth is (i+1)
    uint8 *var;             //! var is the num_bits of data(uint8)
    int size_var;           //len - 19
}JFIF_DHT;

typedef struct _JFIF_SOS
{
    unsigned short marker;  //0xFFDA
    unsigned short len;     //0x000c
    uint8 num_comp;         //0x03
    unsigned short luma;    //0x0100
    unsigned short chroma1; //0x0311
    unsigned short chroma2; //0x0311
    uint8 spectral[3];      //0x003f00
}JFIF_SOS;

class CJfif
{
public:

    CJfif();

    virtual ~CJfif();

    /*
     * @brief create a "jpeg" file
     * @note  ! jfif's attributes should have been set correctly
     */
    void WriteFile(const char* fileName);

    /*
     * @brief read info from a "jpeg" file
     */
    void ReadFile(const char* fileName);

    void SetDQT(const uint8 data[], bool luma);

    void SetSize(int w, int h);    

    void SetDHT(const uint8 bits[], const uint8 var[], int size_var, bool luma, bool dc);    

    void SetData(const uint8 data[], int data_size);

    void GetDQT(bool luma, uint8 data[]);

    void GetSize(int& w, int& h);

    uint8* GetDHT(bool luma, bool dc, uint8 bits[], int& size_var);

    uint8* GetData(int& data_size);

private:

    FILE* fp;

    void WriteByte(uint8 byte);

    // big-endian
    void WriteWord(unsigned short word);

    void ReadByte(uint8& byte);

    // big-endian
    void ReadWord(unsigned short& word);

private:
    
    unsigned short SOI_marker;  //0xFFD8    

    unsigned short EOI_marker;  //0xFFD9

    JFIF_APP APP0;

    JFIF_DQT DQT_Luma, DQT_Chroma;

    JFIF_SOF SOF0;

    JFIF_DHT DHT_Luma_dc, DHT_Luma_ac, DHT_Chroma_dc, DHT_Chroma_ac;

    JFIF_SOS SOS;

    uint8 *data;

    int data_size;
};

#endif
