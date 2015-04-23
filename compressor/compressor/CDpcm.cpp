#include "CDpcm.h"
#include "tool.h"
#include <iostream>
#include <iomanip>
using namespace std;

CDpcm::CDpcm()
    :data(0), size_data(0), result(0)
{
    
}

CDpcm::~CDpcm()
{
    Clear();
}

void CDpcm::Compute(void)
{
    int last = 0;
    for (int i=0; i<size_data; i++)
    {
        result[i] = data[i] - last;
        last = data[i];
    }
}

void CDpcm::SetData(const int data[], int size_data)
{
    ASSERT(0 < size_data);

    // must clear(case: repeatly setData)
    Clear();

    this->size_data = size_data;
    this->data = new int[size_data];
    result = new int[size_data];
    ASSERT((0 != data) && (0 != result));
}

const int* CDpcm::GetResult(void)
{
    return result;
}

void CDpcm::Print(void)
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
    cout << endl << "the result:" << size_data << endl;
    for (int i=0; i<size_data; i++)
    {
        cout << setw(4) << result[i];
        if (0 == (i+1)%16)
        {
            cout << endl;
        }
    }
    cout << endl;
}

void CDpcm::Clear(void)
{
    if (0 != data)
    {
        delete[] data;
        data = 0;
    }
    if (0 != result)
    {
        delete[] result;
        result = 0;
    }
}