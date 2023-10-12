#ifndef DEFINES_H__
#define DEFINES_H__

// null
#ifndef null
#define null NULL
#endif

// bool
#ifndef bool
#define bool u_int8_t
#define true 1
#define false 0
#endif

// strings
#define err "\e[31m[ ERR ]\e[0m"
#define ok "\e[31m[ OK ]\e[0m"

#define min(x, y) ((x) > (y) ? y : x)
#define max(x, y) ((x) < (y) ? y : x)
#endif
