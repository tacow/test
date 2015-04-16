#ifndef LOGGER_MP_H
#define LOGGER_MP_H

#ifdef __unix__
#include "linux/logger.h"
#include "linux/GbToUtf8Modifier.h"
#include "linux/StrReplaceModifier.h"
#endif

#ifdef WIN32
#include "windows/logger.h"
#include "windows/GbToUtf8Modifier.h"
#include "windows/StrReplaceModifier.h"
#endif

#endif

