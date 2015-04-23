#include "CJfif.h"
#include <cstring>

CJfif::CJfif()
    :fp(0),
    data(0),
    data_size(0)
{
    SOI_marker = M_SOI;
    EOI_marker = M_EOI;

    APP0.marker = M_APP0;
    APP0.len = 0x0010;
    APP0.JFIF[0] = 'J';
    APP0.JFIF[1] = 'F';
    APP0.JFIF[2] = 'I';
    APP0.JFIF[3] = 'F';
    APP0.JFIF[4] = '\0';
    APP0.version = 0x0101;
    APP0.density_unit = 0x00;
    APP0.Xdensity = 0x0001;
    APP0.Ydensity = 0x0001;
    APP0.thumbnail = 0x0000;

    DQT_Luma.marker = M_DQT;
    DQT_Luma.len = 0x0043;
    DQT_Luma.id = 0x00;
    memset(DQT_Luma.data, 0x01, 64);

    DQT_Chroma.marker = M_DQT;
    DQT_Chroma.len = 0x0043;
    DQT_Chroma.id = 0x01;
    memset(DQT_Luma.data, 0x01, 64);

    SOF0.marker = M_SOF0;
    SOF0.len = 0x11;
    SOF0.precision = 0x08;
    SOF0.height = 0;
    SOF0.width = 0;
    SOF0.num_comp = 0x03;
    SOF0.Y.id = 0x01;
    SOF0.Y.sample = 0x22;
    SOF0.Y.quant_table_id = 0x00;
    SOF0.Cb.id = 0x02;
    SOF0.Cb.sample = 0x11;
    SOF0.Cb.quant_table_id = 0x01;
    SOF0.Cr.id = 0x03;
    SOF0.Cr.sample = 0x11;
    SOF0.Cr.quant_table_id = 0x01;

    DHT_Luma_dc.marker = M_DHT;
    DHT_Luma_dc.len = 0;
    DHT_Luma_dc.id = 0x00;
    for (int i=0; i<16; i++)
    {
        DHT_Luma_dc.bits[i] = 0;
    }
    DHT_Luma_dc.var = 0;
    DHT_Luma_dc.size_var = 0;
        
    DHT_Luma_ac.marker = M_DHT;
    DHT_Luma_ac.len = 0;
    DHT_Luma_ac.id = 0x01;
    for (int i=0; i<16; i++)
    {
        DHT_Luma_ac.bits[i] = 0;
    }
    DHT_Luma_ac.var = 0;
    DHT_Luma_ac.size_var = 0;
            
    DHT_Chroma_dc.marker = M_DHT;
    DHT_Chroma_dc.len = 0;
    DHT_Chroma_dc.id = 0x10;
    for (int i=0; i<16; i++)
    {
        DHT_Chroma_dc.bits[i] = 0;
    }
    DHT_Chroma_dc.var = 0;
    DHT_Chroma_dc.size_var = 0;
        
    DHT_Chroma_ac.marker = M_DHT;
    DHT_Chroma_ac.len = 0;
    DHT_Chroma_ac.id = 0x11;
    for (int i=0; i<16; i++)
    {
        DHT_Chroma_ac.bits[i] = 0;
    }
    DHT_Chroma_ac.var = 0;
    DHT_Chroma_ac.size_var = 0;
        
    SOS.marker = M_SOS;
    SOS.len = 0x000c;
    SOS.num_comp = 3;
    SOS.luma = 0x0100;
    SOS.chroma1 = 0x0211;
    SOS.chroma2 = 0x0311;
    SOS.spectral[0] = 0x00;
    SOS.spectral[1] = 0x3f;
    SOS.spectral[2] = 0x00;
}

CJfif::~CJfif()
{
    if (0 != data)
    {
        delete[] data;
        data = 0;
    }
    if (0 != DHT_Luma_dc.var)
    {
        delete[] DHT_Luma_dc.var;
        DHT_Luma_dc.var = 0;
    }
    if (0 != DHT_Luma_ac.var)
    {
        delete[] DHT_Luma_ac.var;
        DHT_Luma_ac.var = 0;
    }
    if (0 != DHT_Chroma_dc.var)
    {
        delete[] DHT_Chroma_dc.var;
        DHT_Chroma_dc.var = 0;
    }
    if (0 != DHT_Chroma_ac.var)
    {
        delete[] DHT_Chroma_ac.var;
        DHT_Chroma_ac.var = 0;
    }
}

void CJfif::WriteFile(const char* fileName)
{
    fopen_s(&fp, fileName, "wb");
    ASSERT(0 != fp);

    WriteWord(SOI_marker);

    WriteWord(APP0.marker);
    WriteWord(APP0.len);
    WriteByte(APP0.JFIF[0]);
    WriteByte(APP0.JFIF[1]);
    WriteByte(APP0.JFIF[2]);
    WriteByte(APP0.JFIF[3]);
    WriteByte(APP0.JFIF[4]);
    WriteWord(APP0.version);
    WriteByte(APP0.density_unit);
    WriteWord(APP0.Xdensity);
    WriteWord(APP0.Ydensity);
    WriteWord(APP0.thumbnail);

    JFIF_DQT *dqt[2];
    dqt[0] = &DQT_Luma;
    dqt[1] = &DQT_Chroma;
    for (int i=0; i<2; i++)
    {
        WriteWord(dqt[i]->marker);
        WriteWord(dqt[i]->len);
        WriteByte(dqt[i]->id);
        for (int j=0; j<64; j++)
        {
            WriteByte(dqt[i]->data[j]);
        }
    }

    WriteWord(SOF0.marker);
    WriteWord(SOF0.len);
    WriteByte(SOF0.precision);
    WriteWord(SOF0.height);
    WriteWord(SOF0.width);
    WriteByte(SOF0.num_comp);
    WriteByte(SOF0.Y.id);
    WriteByte(SOF0.Y.sample);
    WriteByte(SOF0.Y.quant_table_id);
    WriteByte(SOF0.Cb.id);
    WriteByte(SOF0.Cb.sample);
    WriteByte(SOF0.Cb.quant_table_id);
    WriteByte(SOF0.Cr.id);
    WriteByte(SOF0.Cr.sample);
    WriteByte(SOF0.Cr.quant_table_id);

    JFIF_DHT *dht[4];
    dht[0] = &DHT_Luma_dc;
    dht[1] = &DHT_Chroma_dc;
    dht[2] = &DHT_Luma_ac;    
    dht[3] = &DHT_Chroma_ac;
    for (int i=0; i<4; i++)
    {
        WriteWord(dht[i]->marker);
        WriteWord(dht[i]->len);
        WriteByte(dht[i]->id);
        for (int j=0; j<16; j++)
        {
            WriteByte(dht[i]->bits[j]);
        }
        for (int j=0; j<dht[i]->size_var; j++)
        {
            WriteByte(dht[i]->var[j]);
        }
    }

    WriteWord(SOS.marker);
    WriteWord(SOS.len);
    WriteByte(SOS.num_comp);
    WriteWord(SOS.luma);
    WriteWord(SOS.chroma1);
    WriteWord(SOS.chroma2);
    WriteByte(SOS.spectral[0]);
    WriteByte(SOS.spectral[1]);
    WriteByte(SOS.spectral[2]);

    for (int i=0; i<data_size; i++)
    {
        WriteByte(data[i]);
    }

    WriteWord(EOI_marker);
    fclose(fp);
}

void CJfif::ReadFile(const char* fileName)
{
    fopen_s(&fp, fileName, "rb");
    ASSERT(0 != fp);
    fseek(fp, 0, SEEK_END);
    int fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ReadWord(SOI_marker);

    ReadWord(APP0.marker);
    ReadWord(APP0.len);
    ReadByte(APP0.JFIF[0]);
    ReadByte(APP0.JFIF[1]);
    ReadByte(APP0.JFIF[2]);
    ReadByte(APP0.JFIF[3]);
    ReadByte(APP0.JFIF[4]);
    ReadWord(APP0.version);
    ReadByte(APP0.density_unit);
    ReadWord(APP0.Xdensity);
    ReadWord(APP0.Ydensity);
    ReadWord(APP0.thumbnail);

    JFIF_DQT *dqt[2];
    dqt[0] = &DQT_Luma;
    dqt[1] = &DQT_Chroma;
    for (int i=0; i<2; i++)
    {
        ReadWord(dqt[i]->marker);
        ReadWord(dqt[i]->len);
        ReadByte(dqt[i]->id);
        for (int j=0; j<64; j++)
        {
            ReadByte(dqt[i]->data[j]);
        }
    }

    ReadWord(SOF0.marker);
    ReadWord(SOF0.len);
    ReadByte(SOF0.precision);
    ReadWord(SOF0.height);
    ReadWord(SOF0.width);
    ReadByte(SOF0.num_comp);
    ReadByte(SOF0.Y.id);
    ReadByte(SOF0.Y.sample);
    ReadByte(SOF0.Y.quant_table_id);
    ReadByte(SOF0.Cb.id);
    ReadByte(SOF0.Cb.sample);
    ReadByte(SOF0.Cb.quant_table_id);
    ReadByte(SOF0.Cr.id);
    ReadByte(SOF0.Cr.sample);
    ReadByte(SOF0.Cr.quant_table_id);

    JFIF_DHT *dht[4];
    dht[0] = &DHT_Luma_dc;
    dht[1] = &DHT_Chroma_dc;
    dht[2] = &DHT_Luma_ac;    
    dht[3] = &DHT_Chroma_ac;
    for (int i=0; i<4; i++)
    {
        ReadWord(dht[i]->marker);
        ReadWord(dht[i]->len);
        ReadByte(dht[i]->id);
        for (int j=0; j<16; j++)
        {
            ReadByte(dht[i]->bits[j]);
        }
        dht[i]->size_var = dht[i]->len - 19;
        dht[i]->var = new uint8[dht[i]->size_var];
        ASSERT(0 != dht[i]->var);
        for (int j=0; j<dht[i]->size_var; j++)
        {
            ReadByte(dht[i]->var[j]);
        }
    }

    ReadWord(SOS.marker);
    ReadWord(SOS.len);
    ReadByte(SOS.num_comp);
    ReadWord(SOS.luma);
    ReadWord(SOS.chroma1);
    ReadWord(SOS.chroma2);
    ReadByte(SOS.spectral[0]);
    ReadByte(SOS.spectral[1]);
    ReadByte(SOS.spectral[2]);

    int curPos = ftell(fp);
    data_size = fileLength - curPos - 2;
    data = new uint8[data_size];
    ASSERT(0 != data);
    for (int i=0; i<data_size; i++)
    {
        ReadByte(data[i]);
    }

    ReadWord(EOI_marker);

    fclose(fp);
}

void CJfif::SetDQT(const uint8 data[], bool luma)
{
    if (true == luma)
    {
        for (int i=0; i<64; i++)
        {
            DQT_Luma.data[i] = data[i];
        }
    }
    else
    {
        for (int i=0; i<64; i++)
        {
            DQT_Chroma.data[i] = data[i];
        }
    }
}

void CJfif::SetSize(int w, int h)
{
    SOF0.width = w;
    SOF0.height = h;
}

void CJfif::SetDHT(const uint8 bits[], const uint8 var[], int size_var, bool luma, bool dc)
{
    if ((true == luma) && (true == dc))
    {
        for (int i=0; i<16; i++)
        {
            DHT_Luma_dc.bits[i] = bits[i];
        }
        DHT_Luma_dc.size_var = size_var;
        DHT_Luma_dc.var = new uint8[size_var];
        ASSERT(0 != DHT_Luma_dc.var);
        for (int i=0; i<size_var; i++)
        {
            DHT_Luma_dc.var[i] = var[i];
        }
        DHT_Luma_dc.len = (2+1+16+size_var);
    }
    else if ((true == luma) && (false == dc))
    {
        for (int i=0; i<16; i++)
        {
            DHT_Luma_ac.bits[i] = bits[i];
        }
        DHT_Luma_ac.size_var = size_var;
        DHT_Luma_ac.var = new uint8[size_var];
        ASSERT(0 != DHT_Luma_ac.var);
        for (int i=0; i<size_var; i++)
        {
            DHT_Luma_ac.var[i] = var[i];
        }
        DHT_Luma_ac.len = (2+1+16+size_var);
    }
    else if ((false == luma) && (true == dc))
    {
        for (int i=0; i<16; i++)
        {
            DHT_Chroma_dc.bits[i] = bits[i];
        }
        DHT_Chroma_dc.size_var = size_var;
        DHT_Chroma_dc.var = new uint8[size_var];
        ASSERT(0 != DHT_Chroma_dc.var);
        for (int i=0; i<size_var; i++)
        {
            DHT_Chroma_dc.var[i] = var[i];
        }
        DHT_Chroma_dc.len = (2+1+16+size_var);
    }
    else
    {
        for (int i=0; i<16; i++)
        {
            DHT_Chroma_ac.bits[i] = bits[i];
        }
        DHT_Chroma_ac.size_var = size_var;
        DHT_Chroma_ac.var = new uint8[size_var];
        ASSERT(0 != DHT_Chroma_ac.var);
        for (int i=0; i<size_var; i++)
        {
            DHT_Chroma_ac.var[i] = var[i];
        }
        DHT_Chroma_ac.len = (2+1+16+size_var);
    }
}

void CJfif::SetData(const uint8 data[], int data_size)
{
    this->data_size = data_size;
    this->data = new uint8[data_size];
    ASSERT(0!=this->data);

    for (int i=0; i<data_size; i++)
    {
        this->data[i] = data[i];
    }
}

void CJfif::GetDQT(bool luma, uint8 data[])
{
    if (true == luma)
    {
        for (int i=0; i<64; i++)
        {
            data[i] = DQT_Luma.data[i];
        }
    }
    else
    {
        for (int i=0; i<64; i++)
        {
            data[i] = DQT_Chroma.data[i];
        }
    }
}

void CJfif::GetSize(int& w, int& h)
{
    w = SOF0.width;
    h = SOF0.height;
}

uint8* CJfif::GetDHT(bool luma, bool dc, uint8 bits[], int& size_var)
{
    uint8 *ret = 0;
    if ((true == luma) && (true == dc))
    {
        for (int i=0; i<16; i++)
        {
            bits[i] = DHT_Luma_dc.bits[i];
        }
        size_var = DHT_Luma_dc.size_var;
        ret = new uint8[size_var];
        ASSERT(0 != ret);
        for (int i=0; i<size_var; i++)
        {
            ret[i] = DHT_Luma_dc.var[i];
        }
    }
    else if ((true == luma) && (false == dc))
    {
        for (int i=0; i<16; i++)
        {
            bits[i] = DHT_Luma_ac.bits[i];
        }
        size_var = DHT_Luma_ac.size_var;
        ret = new uint8[size_var];
        ASSERT(0 != ret);
        for (int i=0; i<size_var; i++)
        {
            ret[i] = DHT_Luma_ac.var[i];
        }
    }
    else if ((false == luma) && (true == dc))
    {
        for (int i=0; i<16; i++)
        {
            bits[i] = DHT_Chroma_dc.bits[i];
        }
        size_var = DHT_Chroma_dc.size_var;
        ret = new uint8[size_var];
        ASSERT(0 != ret);
        for (int i=0; i<size_var; i++)
        {
            ret[i] = DHT_Chroma_dc.var[i];
        }
    }
    else
    {
        for (int i=0; i<16; i++)
        {
            bits[i] = DHT_Chroma_ac.bits[i];
        }
        size_var = DHT_Chroma_ac.size_var;
        ret = new uint8[size_var];
        ASSERT(0 != ret);
        for (int i=0; i<size_var; i++)
        {
            ret[i] = DHT_Chroma_ac.var[i];
        }
    }
    return ret;
}

uint8* CJfif::GetData(int& data_size)
{
    // there should be data existed
    ASSERT(0!=this->data_size);
    data_size = this->data_size;
    
    uint8 *ret = new uint8[this->data_size];
    ASSERT(0!=ret);

    for (int i=0; i<data_size; i++)
    {
        ret[i] = data[i];
    }
    return ret;
}

void CJfif::WriteByte(uint8 byte)
{
    int ret = fwrite(&byte, 1, 1, fp);
    ASSERT(1 == ret);
}

void CJfif::WriteWord(unsigned short word)
{
    WriteByte(uint8(word>>8));
    WriteByte(uint8(word&0xFF));
}

void CJfif::ReadByte(uint8& byte)
{
    int ret = fread(&byte, 1, 1, fp);
    ASSERT(1 == ret);
}

void CJfif::ReadWord(unsigned short& word)
{
    uint8 t1, t2;
    ReadByte(t1);
    ReadByte(t2);
    word = t1;
    word = (word<<8) + t2;
}



