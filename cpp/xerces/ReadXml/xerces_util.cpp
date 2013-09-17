#include "xerces_util.h"

DOMElement* GetChildElem(DOMElement* elem, const char* childName) {
    DOMElement* childElem = elem->getFirstElementChild();
    while(childElem) {
        if (0 == strcmp(C(childElem->getTagName()), childName))
            return childElem;
        childElem = childElem->getNextElementSibling();
    }
    return NULL;
}

DOMElement* GetNextElem(DOMElement* elem, const char* siblingName) {
    DOMElement* siblingElem = elem->getNextElementSibling();
    while(siblingElem) {
        if (0 == strcmp(C(siblingElem->getTagName()), siblingName))
            return siblingElem;
        siblingElem = siblingElem->getNextElementSibling();
    }
    return NULL;
}

