#ifndef XERCES_UTIL_H
#define XERCES_UTIL_H

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
using namespace xercesc;

class X
{
public:
    X(const char* c) {
        x = XMLString::transcode(c);
    }
    
    operator XMLCh*() {
        return x;
    }

    ~X() {
        XMLString::release(&x);
    }
private:
    XMLCh* x;
};

class C
{
public:
    C(const XMLCh* x) {
        c = XMLString::transcode(x);
    }

    operator char*() {
        return c;
    }

    ~C() {
        XMLString::release(&c);
    }
private:
    char* c;
};

DOMElement* GetChildElem(DOMElement* elem, const char* childName);
DOMElement* GetNextElem(DOMElement* elem, const char* siblingName);

#endif

