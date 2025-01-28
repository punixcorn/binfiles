#include "includes.hpp"
// holdes definitions
#include "defines.h"

namespace ERR {

/*
 * syntax error message and exit
 * @extrainfo is default to empty string
 * @extrainfo displays what you write in as [INFO]...
 */
inline void syntaxerror(int line, std::string& errline,
                        std::string&& extrainfo) {
    int i = 0;
    std::string squiglyline{};
    while (i < errline.size()) {
        squiglyline.push_back('~');
        i++;
    }
    if (extrainfo != "")
        fmt::print(
            "{}[ERR]{} syntax Error found\nLine "
            "{}\n\t{}\n\t{}{}{}\n{}[INFO]{} {}\n",
            red, nocol, line + 1, errline, red, squiglyline, nocol, green,
            nocol, extrainfo);
    else
        fmt::print(
            "{}[ERR]{} syntax Error found\nLine "
            "{}\n\t{}\n\t{}{}{}\n",
            red, nocol, line + 1, errline, red, squiglyline, nocol, green,
            nocol);
    exit(2);
}

inline void exitErr(std::string&& errmessage, int errnum = 1) {
    fmt::print(stderr, "{}[ERR]{} {}\n", red, nocol, errmessage);
    exit(errnum);
}
}  // namespace ERR
