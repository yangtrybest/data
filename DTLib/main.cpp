#include <iostream>
#include "DTString.h"
#include <cstring>
using namespace std;
using namespace DTLib;
int* make_pmt(const char* p)        // O(n)
{
    int len = strlen(p);
    int* ret = static_cast<int*>(malloc(sizeof(int)*len));

    if( ret != NULL )
    {
        int ll = 0;
        ret[0] = 0;
        for(int i=1; i<len; i++)
        {
            while( (ll > 0) && (p[ll] != p[i]) )
            {
                ll = ret[ll - 1];
            }
            if( p[ll] == p[i] )
            {
                ll++;
            }
            ret[i] = ll;
        }
    }
    return ret;
}
int kmp(const char* s, const char* p)       // O(n)
{
    int ret = -1;
    int sl = strlen(s);
    int pl = strlen(p);
    int* pmt = make_pmt(p);
    if( (pmt != NULL) && (0 < pl) && (pl <= sl))
    {
        for(int i=0, j=0; i < sl; i++)
        {
            while( (j > 0) && (s[i] != p[j]) )
            {
                j = pmt[j - 1];
            }
            if( s[i] == p[j] )
            {
                j++;
            }
            if( j == pl )
            {
                ret = i + 1 - pl;
                break;
            }
        }
    }

    free(pmt);
    return ret;
}
int main()
{
    cout << kmp("abcde", "cde") << endl;
    cout << kmp("ababax", "ba") << endl;
    cout << kmp("ababax", "") << endl;
    cout << kmp("ababax", "ababax") << endl;
    cout << kmp("ababax", "ababaxy") << endl;
    return 0;
}
