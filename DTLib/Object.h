#ifndef OBJECT_H
#define OBJECT_H

namespace DTLib{

class Object
{
public:
    void* operator new (unsigned int size) throw();
    void operator delete (void* p);
    void* operator new[] (unsigned int size) throw();
    void operator delete[] (void* p);

    bool operator == (const Object& object);
    bool operator != (const Object& object);
    virtual ~Object()= 0;
};

}

#endif // OBJECT_H
