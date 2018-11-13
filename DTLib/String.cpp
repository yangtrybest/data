#include <cstring>
#include <cstdlib>
#include "DTString.h"
#include "Exception.h"
using namespace std;

namespace DTLib
{

void String::init(const char* s)
{
    m_str = strdup(s);
    if( m_str )
    {
        m_length = strlen(s);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create String object ...");
    }
}
bool String::equal(const char* l, const char* r, int len) const
{
    bool ret = true;

    for(int i=0; i<len && ret; i++)
    {
        ret = ret && (l[i] == r[i]);
    }
    return ret;
}
// 构造函数
String::String()
{
    init("");
}
String::String(const char* s)
{
    init(s ? s : "");
}
String::String(const String& s)
{
    init(s.m_str);
}
String::String(char c)
{
    char s[] = {c, '\0'};
    init(s);
}

// 取长、取字符串
int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}

// 比较操作符重载
bool String::operator ==(const String& s) const
{
    return (strcmp(m_str,s.m_str) == 0);
}
bool String::operator ==(const char* s) const
{
    return (strcmp(m_str,s ? s : "") == 0);
}
bool String::operator !=(const String& s) const
{
    return !(*this == s);
}
bool String::operator !=(const char* s)const
{
    return !(*this == s);
}
bool String::operator > (const String& s)const
{
    return (strcmp(m_str, s.m_str) > 0);
}
bool String::operator > (const char* s)const
{
    return (strcmp(m_str, s ? s : "") > 0);
}
bool String::operator < (const String& s)const
{
    return (strcmp(m_str, s.m_str) < 0);
}
bool String::operator < (const char* s)const
{
    return (strcmp(m_str, s ? s : "") < 0);
}
bool String::operator >=(const String& s)const
{
    return (strcmp(m_str, s.m_str) >=0 );
}
bool String::operator >=(const char* s)const
{
    return (strcmp(m_str, s ? s : "") >= 0);
}
bool String::operator <=(const String& s)const
{
    return (strcmp(m_str, s.m_str) <= 0);
}
bool String::operator <=(const char* s)const
{
    return (strcmp(m_str, s ? s : "") <= 0);
}

// +操作符
String String::operator + (const String& s) const
{
    return (*this + s.m_str);
}
String String::operator + (const char* s) const
{
    String ret;
    int len = m_length + strlen(s ? s : "");
    char* str = reinterpret_cast<char*>(malloc(len + 1));

    if(str)
    {
        strcpy(str, m_str);
        strcat(str, s ? s : "");

        free(ret.m_str);

        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to add string values ...");
    }
    return ret;
}

String& String::operator += (const String& s)
{
    return (*this = *this + s.m_str);
}
String& String::operator += (const char* s)
{
    return (*this = *this + s);
}

// 减法操作符
String String::operator - (const String& s) const
{
    return String(*this).remove(s);
}
String String::operator - (const char* s)   const
{
    return String(*this).remove(s);         // 不改变原来的字符串
}
String& String::operator -= (const String& s)
{
    return remove(s);                       // 改变了原来的字符串
}
String& String::operator -= (const char* s)
{
    return remove(s);
}


// 赋值操作符
String& String::operator = (const String& s)
{
    return (*this = s.m_str);
}
String& String::operator = (const char* s)
{
    if( m_str != s)
    {
        char* str = strdup(s ? s : "");
        if( str )
        {
            free( m_str );

            m_str = str;
            m_length = strlen(m_str);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to assign new String value ...");
        }
    }
    return *this;
}
String& String::operator = (char c)
{
    char s[] = {c, '\0'};

    return (*this = s);
}

// []操作符
char& String::operator[] (int i)
{
    if( (0 <= i ) && (i < m_length))
    {
        return m_str[i];
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
}
char  String::operator[] (int i) const
{
    return (const_cast<String&>(*this))[i];
}

// 功能函数
bool String::startWith(const char* s)       const
{
    bool ret = (s != NULL);

    if( ret )
    {
        int len = strlen(s);

        ret = (len < m_length) && equal(m_str, s, len);
    }

    return ret;
}
bool String::startWith(const String& s)     const
{
    return startWith(s.m_str);
}
bool String::endOf(const char* s)           const
{
    bool ret = (s != NULL);

    if( ret )
    {
        int len = strlen(s);
        char* str = m_str + (m_length - len);

        ret = (len < m_length) && equal(str, s, len);
    }

    return ret;
}
bool String::endOf(const String& s)         const
{
    return endOf(s.m_str);
}

// 插入字符串
String& String::insert(int i, const char* s)
{
    if( (0 <= i ) && (i <= m_length))
    {
        if((s != NULL)&&(s[0] != '\0'))
        {
            int len = strlen(s);
            char* str = reinterpret_cast<char*>(malloc(m_length + len + 1));

            if( str != NULL )
            {
                strncpy(str, m_str, i);
                strncpy(str + i, s, len);
                strncpy(str + i + len, m_str + i, m_length - i);

                str[m_length + len] = '\0';
                free(m_str);

                m_str = str;
                m_length = m_length + len;
            }
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
    return *this;
}
String& String::insert(int i, const String& s)
{
    return insert(i, s.m_str);
}
// 删除首尾空格
String& String::trim()
{
    int b = 0;
    int e = m_length - 1;

    while ( m_str[b] == ' ' ) b++;
    while ( m_str[e] == ' ' ) e--;
    if( b == 0 )
    {
        m_str[e + 1] = '\0';
        m_length = e + 1;
    }
    else
    {
        for(int i=0, j=b; j<=e; i++, j++)
        {
            m_str[i] = m_str[j];
        }

        m_str[e - b + 1] = '\0';
        m_length = e - b + 1;
    }
    return *this;
}
// KMP算法中部分匹配表的查找
int* String::make_pmt(const char* p)        // O(n)
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

// KMP算法
int String::kmp(const char* s, const char* p)       // O(n)
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
// 索引函数
int String::indexOf(const char* s)          const
{
    return kmp(m_str, s ? s : "");
}
int String::indexOf(const String& s)        const
{
    return kmp(m_str, s.m_str);
}

// 删除函数
String& String::remove(int i, int len)
{
    if( (0 <= i)&&(i < m_length))
    {
        int n = i;
        int m = i + len;

        while ((n < m)&&(m < m_length))
        {
            m_str[n++] = m_str[m++];
        }

        m_str[n] = '\0';
        m_length = n;
    }
    return *this;
}
String& String::remove(const char* s)
{
    return remove(indexOf(s), s ? strlen(s) : 0);
}
String& String::remove(const String& s)
{
    return remove(indexOf(s), s.length());
}

// 替换函数
String& String::replace(const char* t, const char* s)
{
    int index = indexOf(t);

    if( index > 0 )
    {
        remove(t);
        insert(index, s);
    }
    return *this;
}
String& String::replace(const String& t, const char* s)
{
    return replace(t.m_str, s);
}
String& String::replace(const char* t, const String& s)
{
    return replace(t, s.m_str);
}
String& String::replace(const String& t, const String& s)
{
    return replace(t.m_str, s.m_str);
}

// 从原字符串中创建子串
String String::sub(int i, int len) const
{
    String ret;

    if( (0 <= i)&&(i < m_length) )
    {
        if( len < 0 ) len = 0;
        if( len + i > m_length ) len = m_length - i;
        char* str = reinterpret_cast<char*>(malloc(len + 1));
        if( str )
        {
            strncpy(str, m_str + i, len);
            str[len] = '\0';
            ret = str;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create substring ...");
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
    return ret;
}
String::~String()
{
    free(m_str);
}

}
