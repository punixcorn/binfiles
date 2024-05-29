#include "errormessage.h"

/*
 * syntax error message and exit
 * @extrainfo is default to empty string
 * @extrainfo displays what you write in as [INFO]...
 */
void ERR::syntaxerror(int line, std::string& errline, std::string&& extrainfo) {
    int i = 0;
    std::string squiglyline{};
    while (i < errline.size()) {
        squiglyline.push_back('~');
        i++;
    }
    if (extrainfo != "")
        std::print(
            "{}[ERR]{} syntax Error found\nLine "
            "{}\n\t{}\n\t{}{}{}\n{}[INFO]{} {}\n",
            red, nocol, line + 1, errline, red, squiglyline, nocol, green,
            nocol, extrainfo);
    else
        std::print(
            "{}[ERR]{} syntax Error found\nLine "
            "{}\n\t{}\n\t{}{}{}\n",
            red, nocol, line + 1, errline, red, squiglyline, nocol, green,
            nocol);
    exit(2);
}

void ERR::exitErr(std::string&& errmessage, int errnum) {
    std::print(stderr, "{}[ERR]{} {}\n", red, nocol, errmessage);
    exit(errnum);
}
