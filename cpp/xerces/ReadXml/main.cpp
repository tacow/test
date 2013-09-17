#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>
#include "xerces_util.h"

using namespace std;

void Parse(const char* xmlContent, size_t contentLen) {
    string a, b;
    
    auto_ptr<XercesDOMParser> parser(new XercesDOMParser());
    auto_ptr<ErrorHandler> errHandler(new HandlerBase());
    parser->setErrorHandler(errHandler.get());
    try {
        MemBufInputSource is((const XMLByte*)xmlContent, contentLen, (XMLCh*)NULL);
        parser->parse(is);
        DOMDocument* doc = parser->getDocument();
        DOMElement* docElem = doc->getDocumentElement();

        DOMElement* aElem = GetChildElem(docElem, "a");
        a = (char*)C(aElem->getTextContent());

        DOMElement* bElem = GetChildElem(docElem, "b");
        b = (const char*)C(bElem->getAttribute(X("name")));
    }
    catch (...) {
    }

    cout << "a: " << a << ", b: " << b << endl;
}

int main(int argc, char* argv[]) {
    XMLPlatformUtils::Initialize();

    char xmlContent[4096];
    FILE* f = fopen("1.xml", "r");
    size_t contentLen = fread(xmlContent, 1, 4096, f);
    fclose(f);

    Parse(xmlContent, contentLen);

    XMLPlatformUtils::Terminate();
    return 0;
}

