#include "CRle.h"
#include "tool.h"
#include <iostream>
#include <iomanip>
using namespace std;

CRle::CRle()
    :data(0), size_data(0), pair(0), num_pair(0)
{
    
}

CRle::~CRle()
{
    Clear();
}

void CRle::Compute(void)
{
    int p = 0;
    int run_len = 0;
    for (int i = 0; i < size_data; i++)
    {
        if (0 == data[i])
        {
            run_len++;
        }
        else
        {
            pair[p].num_zero = run_len;
            pair[p].var = data[i];
            p++;
            run_len = 0;
        }
    }
    num_pair = p;
}

void CRle::SetData(const int data[], int size_data)
{
    ASSERT(0 < size_data);

    // must clear(case: repeatly SetData)
    Clear();

    this->size_data = size_data;
    this->data = new int[size_data];
    pair = new RLE_PAIR[size_data];
    ASSERT((0 != data) && (0 != pair));
}

const RLE_PAIR* CRle::GetPair(void)
{
    return pair;
}

void CRle::Print(void)
{
    cout << "the input: " << size_data << endl;
    for (int i=0; i<size_data; i++)
    {
        cout << setw(4) << data[i];
        if (0 == (i+1)%16)
        {
            cout << endl;
        }
    }
    cout << endl << "the result:" << num_pair << endl;
    for (int i=0; i<num_pair; i++)
    {
        cout << "(" << setw(2) << pair[i].num_zero << "," << setw(4) << pair[i].var << ") ";
        if (0 == (i+1)%8)
        {
            cout << endl;
        }
    }
    cout << endl;
}

void CRle::Clear(void)
{
    if (0 != data)
    {
        delete[] data;
        data = 0;
    }
    if (0 != pair)
    {
        delete[] pair;
        pair = 0;
    }
}
