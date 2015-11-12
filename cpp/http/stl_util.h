#ifndef STL_UTIL_H
#define STL_UTIL_H

#include <string>
#include <sstream>

using namespace std;

template<class T>
void Delete(T* obj) {
    delete obj;
}

template<class T>
void Release(T* obj) {
    obj->Release();
}

template<class Container>
void ContainerDelete(Container& c) {
    for(typename Container::iterator it = c.begin(); it != c.end(); ++it)
        delete *it;
}

template<class Container>
void ContainerRelease(Container& c) {
    for(typename Container::iterator it = c.begin(); it != c.end(); ++it)
        (*it)->Release();
}

template<class Container>
string ContainerToString(Container& c, const char* sep) {
    bool first = true;
    ostringstream oss;
    for(typename Container::iterator it = c.begin(); it != c.end(); ++it) {
        if (first)
            first = false;
        else
            oss << sep;

        oss << *it;
    }
    return oss.str();
}

template<class Container>
string PtrContainerToString(Container& c, const char* sep) {
    bool first = true;
    ostringstream oss;
    for(typename Container::iterator it = c.begin(); it != c.end(); ++it) {
        if (first)
            first = false;
        else
            oss << sep;

        oss << **it;
    }
    return oss.str();
}

template<class Container, class ToStringFunc>
string ContainerToString(Container& c, ToStringFunc& ToString, const char* sep) {
    bool first = true;
    ostringstream oss;
    for(typename Container::iterator it = c.begin(); it != c.end(); ++it) {
        if (first)
            first = false;
        else
            oss << sep;

        oss << ToString(*it);
    }
    return oss.str();
}

template<class Container, class ToStringFunc>
string PtrContainerToString(Container& c, ToStringFunc& ToString, const char* sep) {
    bool first = true;
    ostringstream oss;
    for(typename Container::iterator it = c.begin(); it != c.end(); ++it) {
        if (first)
            first = false;
        else
            oss << sep;

        oss << ToString(**it);
    }
    return oss.str();
}

template<class T>
int Round(T x) {
    return int(x + (T)0.5);
}

template<class T, class D>
void Round(T x, D& i) {
    i = (D)(x + (T)0.5);
}

string Trim(const string& str);

#endif

