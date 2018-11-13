#ifndef STRING_H
#define STRING_H
#include "Object.h"

namespace DTLib
{
class String : public Object
{
protected:
    char* m_str;
    int m_length;

    // 初始化函数
    void init(const char* s);
    // 比对函数
    bool equal(const char* l, const char* r, int len) const;

    static int kmp(const char* s, const char* p);
    static int* make_pmt(const char* p);
public:
    String();
    String(const char* s);
    String(const String& s);
    String(char c);
    int length() const;
    const char* str() const;

    bool operator ==(const String& s)   const;
    bool operator ==(const char* s)     const;
    bool operator !=(const String& s)   const;
    bool operator !=(const char* s)     const;
    bool operator > (const String& s)   const;
    bool operator > (const char* s)     const;
    bool operator < (const String& s)   const;
    bool operator < (const char* s)     const;
    bool operator >=(const String& s)   const;
    bool operator >=(const char* s)     const;
    bool operator <=(const String& s)   const;
    bool operator <=(const char* s)     const;

    String operator + (const String& s) const;
    String operator + (const char* s)   const;
    String& operator += (const String& s);
    String& operator += (const char* s)  ;

    String operator - (const String& s) const;
    String operator - (const char* s)   const;
    String& operator -= (const String& s);
    String& operator -= (const char* s)  ;

    String& operator = (const String& s);
    String& operator = (const char* s);
    String& operator = (char c);

    char& operator[] (int i);
    char  operator[] (int i)            const;

    bool startWith(const char* s)       const;
    bool startWith(const String& s)     const;
    bool endOf(const char* s)           const;
    bool endOf(const String& s)         const;
    String& trim();
    String& insert(int i, const char* s);
    String& insert(int i, const String& s);

    int indexOf(const char* s)          const;
    int indexOf(const String& s)        const;
    String& remove(int i, int len);
    String& remove(const char* s);
    String& remove(const String& s);

    String& replace(const char* t, const char* s);
    String& replace(const String& t, const char* s);
    String& replace(const char* t, const String& s);
    String& replace(const String& t, const String& s);

    String sub(int i, int len) const;
    ~String();
};

}
#endif // STRING_H
