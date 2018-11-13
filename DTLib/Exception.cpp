#include "Exception.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
namespace DTLib{
//void Exception::itoa(int line, char *sl, int n)
//{
//    if(sl != NULL)
//    {
//        int count = 1;
//        int temp = line / n;
//        while(temp > 0)
//        {
//            temp = temp / n;
//            count++;
//        }
//        temp = line;
//        while(count > 0)
//        {
//            char tem = temp % n + 48;
//            sl[count - 1] = tem;
//            temp = temp / n;
//            count--;
//        }
//    }
//}
void Exception::init(const char* message, const char* file, int line)
{
    //1 ����һ���ַ��������ݣ���Ϊ��ȷ��message�洢�����2 Ϊ���쳣��ȫ
    //������glibc2.20�У�strdup(s) û���ж�s�Ƿ�Ϊ�գ����Ը����£�
    m_message = ((message)?strdup(message) : NULL);
    if( file != NULL )
    {
        char sl[16] = {0};
        itoa(line, sl, 10); // 10 ��ʾ ʮ����
        m_location = static_cast<char*>(malloc(strlen(file)+strlen(sl)+2));

        if ( m_location != NULL )
        {
            m_location = strcpy(m_location, file);
            m_location = strcat(m_location, ":");
            m_location = strcat(m_location, sl);
        }
    }
    else
    {
        m_location = NULL;
    }
}


Exception::Exception(const char* message)
{
    init(message, NULL, 0);
}
Exception::Exception(const char* file, int line)
{
    init(NULL, file, line);
}
Exception::Exception(const char* message, const char* file, int line)
{
    init(message, file, line);
}
// ���е�������������Ҫʵ��������������
Exception::Exception(const Exception& e)
{
    m_message = strdup(e.m_message);
    m_location = strdup(e.m_location);
}
Exception& Exception::operator = (const Exception& e)
{
    if( this != &e )
    {
        free(m_message);
        free(m_location);

        m_message = strdup(e.m_message);
        m_location = strdup(e.m_location);
    }

    return *this;
}

const char* Exception::message() const
{
    return m_message;
}
const char* Exception::location() const
{
    return m_location;
}
Exception::~Exception()
{
    free(m_message);
    free(m_location);
}

}
