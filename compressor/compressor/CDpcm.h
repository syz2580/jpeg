#ifndef _CDPCM_H_
#define _CDPCM_H_

// differential pulse code modulation(DPCM), as a simple class to simplify "alloc and free" operations
class CDpcm
{
public:

    CDpcm();

    virtual ~CDpcm();

    void Compute(void);

    void SetData(const int data[], int size_data);

    const int* GetResult(void);

    int GetSize() { return size_data; }

    void Print(void);
    
private:

    void Clear(void);

    int size_data;  //in

    int* data;      //in    

    int* result;
};

#endif