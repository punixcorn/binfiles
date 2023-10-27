#ifndef __GITALIAS_DEFINES_
#define __GITALIAS_DEFINES_

#ifndef null
#define null NULL
#endif
#define arg(cmd, type) args[cmd].as<type>()
#define red "\e[42m"
#define green "\e[43m"
#define blue "\e[45m"
#define rs "\e[0m"
#endif // !__GITALIAS_DEFINES_
