#include "TokenReader.h"
#include <stdio.h>

int main() {
    const char* str = "abc,def;ghi,jkl,mno;pqr";

    TokenReader outerTokenReader;
    outerTokenReader.CopyInit(str, ";");
    
    char* outerToken = NULL;
    while((outerToken = outerTokenReader.ReadToken())) {
        TokenReader innerTokenReader;
        innerTokenReader.Init(outerToken, ",");
        
        char* innerToken = NULL;
        while((innerToken = innerTokenReader.ReadToken())) {
            printf("%s\n", innerToken);
        }
    }

    return 0;
}

