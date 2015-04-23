#ifndef _CIMAGE_H_
#define _CIMAGE_H_

template <class T>
class CImage
{
public:    

    CImage();

    CImage(int w, int h);   // when size of y channel and c channels is same

    CImage(int yw, int yh, int cw, int ch); // not same

    virtual ~CImage();

    void Print(int maxCol);

public: //temp solution
    
    T *y, *cb, *cr; // 3 components

    int yw, yh;     // size of "y" component

    int cw, ch;     // size of "c" component
};

template <class T>
CImage<T>::CImage()
    :y(0),cb(0),cr(0),yw(0),yh(0),cw(0),ch(0)
{
    
}

template <class T>
CImage<T>::CImage(int w, int h)
    :y(0),cb(0),cr(0),yw(w),yh(h),cw(w),ch(h)
{
    y = new T[yw*yh];
    cb = new T[cw*ch];
    cr = new T[cw*ch];
    ASSERT((0!=y)&&(0!=cb)&&(0!=cr));
}

template <class T>
CImage<T>::CImage(int yw, int yh, int cw, int ch)
    :y(0),cb(0),cr(0),yw(yw),yh(yh),cw(cw),ch(ch)
{
    y = new T[yw*yh];
    cb = new T[cw*ch];
    cr = new T[cw*ch];
    ASSERT((0!=y)&&(0!=cb)&&(0!=cr));
}

template <class T>
CImage<T>::~CImage()
{
    if (0 != y)
    {
        delete[] y;
        y = 0;
    }
    if (0 != cb)
    {
        delete[] cb;
        cb = 0;
    }
    if (0 != cr)
    {
        delete[] cr;
        cr = 0;
    }
}

template<class T>
void CImage<T>::Print(int maxCol)
{
    cout << "y:" << endl;
    for (int i=0; i<yh; i++)
    {
        for (int j=0; j<yw; j++)
        {
            cout << setw(4) << static_cast<int>(y[i*yw+j]);
            if ((0 != maxCol) && (0 == (j+1)%maxCol))
            {
                cout << endl;
            }
        }
        cout << endl;
    }
    cout << "cb:" << endl;
    for (int i=0; i<ch; i++)
    {
        for (int j=0; j<cw; j++)
        {
            cout << setw(4) << static_cast<int>(cb[i*cw+j]);
            if ((0 != maxCol) && (0 == (j+1)%maxCol))
            {
                cout << endl;
            }
        }
        cout << endl;
    }
    cout << "cr:" << endl;
    for (int i=0; i<ch; i++)
    {
        for (int j=0; j<cw; j++)
        {
            cout << setw(4) << static_cast<int>(cr[i*cw+j]);
            if ((0 != maxCol) && (0 == (j+1)%maxCol))
            {
                cout << endl;
            }
        }
        cout << endl;
    }
}


#endif


