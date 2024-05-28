#ifndef __ERRORMESSAGE_H__
#define __ERRORMESSAGE_H__
#include "Makezip.h"
#include "defines.h"
#include "zip.h"
namespace ERR {
void syntaxerror(int line, std::string& errline, std::string&& extrainfo = "");
void exitErr(std::string&& errmessage, int errnum = 1);
}  // namespace ERR
#endif
