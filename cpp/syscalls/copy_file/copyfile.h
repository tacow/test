#ifndef COPYFILE_H
#define COPYFILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>

using namespace std;

bool CopyFile(const string& srcPath, const string& dstPath, off_t offset, size_t len, string& errMsg, mode_t mode = 0644, bool checkFileSize = true);
bool CopyFile(const string& srcPath, const string& dstPath, string& errMsg, mode_t mode = 0644);

#endif
