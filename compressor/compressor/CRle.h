#ifndef _CRLE_H_
#define _CRLE_H_

typedef struct _RLE_PAIR
{
    int num_zero;
    int var;
}RLE_PAIR;

// Run-length encoding as a simple class
class CRle
{
public:    
    
    CRle();

    virtual ~CRle();

    void Compute(void);    

    void SetData(const int data[], int size_data);

    const RLE_PAIR* GetPair(void);

    void Print(void);

private:

    void Clear();

    int* data;      //the input when RLE

    int size_data;

    RLE_PAIR *pair; //the output when RLE

    int num_pair;
};

#endif