#ifndef __DEFINES_H___
#define __DEFINES_H___

// null errors
#ifndef null
#define null NULL
#endif

#define setup_va(last_variable) \
    va_list ap;                 \
    va_start(ap, last_variable);
#define va_get(type) va_arg(ap, type);

/* colors for terminal */
#define red "\e[35m"
#define nocol "\e[0m"
#define green "\e[33m"
#define yellow "\e[34m"
#define bold "\e[1m"

#endif
