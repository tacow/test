#ifndef COPYFILE_H
#define COPYFILE_H

#include <stdio.h>
#include <errno.h>
#include <string>

using namespace std;

bool MoveFile(const string& srcPath, const string& dstPath, string& errMsg);

#endif
