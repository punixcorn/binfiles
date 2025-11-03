#include <assert.h>

#include <algorithm>
#include <print>
#include <string>
#include <vector>

#define err "\e[31m[ ERR ]\e[0m"
#define ok "\e[22m[ OK ]\e[0m"

namespace hdmi {

enum arg_size { small, big };
enum exit_mode { normal, help };

struct info {
    std::string monitor;   // monitor name
    std::string Mainsize;  // main monitor size
    std::string Hdmisize;  // hdmi monitor size
    std::string place;     // where hdmi should be relative to main
    bool show;             // show more information
    bool duplicate;
    bool verbose;

    info() {
        monitor = "";
        Mainsize = "";
        Hdmisize = "";
        place = "";
        show = false;
        duplicate = false;
        verbose = false;
    }
};

std::string xrandr_info() {
    FILE *fd = popen("xrandr", "r");
    char *buffer = (char *)malloc(3048);
    if (buffer == NULL) {
        std::print(stderr, "Unable to allocate space for buffer\n");
        exit(1);
    }
    assert(fread(buffer, sizeof(char), 3048, fd));
    pclose(fd);
    return std::string(buffer);
}

std::string xrandr_displays() {
    FILE *fd = popen(
        "xrandr --listactivemonitors | grep \"+\"  | awk '{ print $NF }'", "r");

    char *buffer = (char *)malloc(1024);
    if (buffer == NULL) {
        std::print(stderr, "Unable to allocate space for buffer\n");
        exit(1);
    }
    assert(fread(buffer, sizeof(char), 1024, fd));
    pclose(fd);
    return std::string(buffer);
}

bool is_option(std::string &str) {
    if (str.size() > 0 && str[0] == '-') return true;
    return false;
}

static void Exit(std::string program_name, hdmi::exit_mode mode) {
    if (mode == hdmi::exit_mode::normal) {
        std::print(
            stderr,
            "Invalid arguments passed\ntry {} --help for more information",
            program_name);
        exit(1);
    } else if (mode == hdmi::exit_mode::help) {
        std::print(
            stdout,
            "{}: Used to setup dual displays\n"
            "--help  -h                      print this help message\n"
            "-m [monitor name]               default is eDP1\n"
            "--Mainsize  -M [size]           specify a Main size  [ default: "
            "1920x1080 ]\n"
            "--Hdmisize  -H [size]           specify an HDMI size [ default: "
            "1920x1080 ]\n"
            "--show  -s                      ouput xrandr\n"
            "--place -p [right/left/up/down] where HDMI monitor should be "
            "placed relative to main monitor\n"
            "--off -o                        turn of HDMI Display\n"
            "--dup -d                        duplicate Display\n"
            "--verbose -v                    Internal Information\n",
            program_name);
        exit(0);
    }
    exit(0);
}

bool is_hdmi_connected() {
    auto offbuffer = xrandr_info();
    if (offbuffer.find("HDMI1 disconnected (normal left inverted right x "
                       "axis y axis)") != std::string::npos)
        return false;
    return true;
}

};  // namespace hdmi

void setup(hdmi::info &Io, std::vector<std::string> argv, int &i,
           const hdmi::arg_size op) {
    char temp;

    if (op == hdmi::arg_size::small)
        temp = argv[i][1];
    else if (op == hdmi::arg_size::big)
        temp = argv[i][2];

    switch (temp) {
        case 'm':
            i += 1;
            if (hdmi::is_option(argv[i]))
                hdmi::Exit(argv[0], hdmi::exit_mode::normal);
            Io.monitor = argv[i];
            if (Io.monitor.empty()) {
                std::print("Could not read monitor name\n");
                exit(1);
            }
            break;
        case 'M':
            i += 1;
            if (hdmi::is_option(argv[i]))
                hdmi::Exit(argv[0], hdmi::exit_mode::normal);
            Io.Mainsize = argv[i];
            if (Io.Mainsize.empty()) {
                std::print("Could not read display Main size\n");
                exit(1);
            }
            break;
        case 'H':
            i += 1;
            if (hdmi::is_option(argv[i]))
                hdmi::Exit(argv[0], hdmi::exit_mode::normal);
            Io.Hdmisize = argv[i];
            if (Io.Hdmisize.empty()) {
                std::print("Could not read display HDMI size\n");
                exit(1);
            }
            break;
        case 's':
            if (argv[i] == "--show" || argv[i] == "-s") {
                Io.show = true;
            }
            break;
        case 'p':
            i += 1;
            if (hdmi::is_option(argv[i]))
                hdmi::Exit(argv[0], hdmi::exit_mode::normal);
            Io.place = argv[i];
            if (Io.place.empty()) {
                std::print("Could not read HDMI display position size\n");
                exit(1);
            }
            break;
        case 'h':
            hdmi::Exit(argv[0], hdmi::exit_mode::help);
            break;
        case 'd':
            Io.duplicate = true;
            break;
        case 'v':
            Io.verbose = true;
            break;
        case 'o':
            if (hdmi::is_hdmi_connected() == false) {
                std::print(stderr, err
                           " no HDMI setup found\nTry running"
                           "\"xrandr --output HDMI1 --off\"\n");
            } else {
                system("xrandr --output HDMI1 --off");
                std::print(ok " HDMI settings off\n");
            }
            exit(0);
            break;
        default:
            hdmi::Exit(argv[0], hdmi::exit_mode::normal);
            break;
    }
}

int main(int argc, char **argv) {
    std::vector<std::string> sargv(argv, argv + argc);

    hdmi::info Io;

    int i{1};

    while (i < argc) {
        int count{0};
        if (sargv[i].substr(0, 2) == "--")
            setup(Io, sargv, i, hdmi::arg_size::big);
        if (sargv[i].substr(0, 2) != "--" && sargv[i][0] == '-')
            setup(Io, sargv, i, hdmi::arg_size::small);

        i++;
    }

    std::string xrandr_buffer = hdmi::xrandr_info();

    if (Io.show) {
        std::print("[buffer]\n{}\n[EOF]\n", xrandr_buffer);
        exit(0);
    }

    Io.monitor = (Io.monitor.empty() ? "eDP1" : Io.monitor);
    Io.Hdmisize = (Io.Hdmisize.empty() ? "1920x1080" : Io.Hdmisize);
    Io.Mainsize = (Io.Mainsize.empty() ? "1920x1080" : Io.Mainsize);

    if (Io.duplicate) {
        Io.place = "--same-as ";
    } else {
        if (!Io.place.empty()) {
            char c = Io.place[0];
            switch (tolower(c)) {
                case 'r':
                    Io.place = "--right-of ";
                    break;
                case 'l':
                    Io.place = "--left-of ";
                    break;
                case 'u':
                    Io.place = "--above ";
                    break;
                case 'd':
                    Io.place = "--below ";
                    break;
                default:
                    std::print(
                        "\e[31m[ Warning ] Uknown option {} used in --place "
                        "using: right\e[0m\n",
                        Io.place);
                    Io.place = "--right-of ";
                    break;
            }
        } else {
            std::print("\e[31m[ Info ] default position: right\e[0m\n");
            Io.place = "--right-of ";
        }
    }

    auto displays = hdmi::xrandr_displays();

    if (displays.find(Io.monitor) != std::string::npos) {
        std::print("{} display {} found\n", ok, Io.monitor);

    } else {
        std::print(stderr, "{} display {} not found\n", err, Io.monitor);
        exit(1);
    }

    if (xrandr_buffer.find("HDMI1 connected") != std::string::npos) {
        std::print("{} HDMI connection found\n", ok);
    } else {
        std::print(stderr, "{} HDMI connection not found\n", err);
        exit(1);
    }

    auto cmd = std::format(
        "xrandr --output {} --primary --mode {} --rotate normal "
        "--output HDMI1 --mode {} --rotate normal {} {}",
        Io.monitor, Io.Mainsize, Io.Hdmisize, Io.place, Io.monitor);

    if (Io.verbose == true) {
        std::print(
            "[Verbose Info]\n"
            "\t[Main Display]\t{}\n"
            "\t[Main size]\t{}\n"
            "\t[Hdmi size]\t{}\n"
            "\t[Command]\t{}\n"
            "[EOF]\n",
            Io.monitor, Io.Mainsize, Io.Hdmisize, cmd);
    }

    std::system(cmd.c_str());
    std::print("{} HDMI Setup completed\n", ok);
    return 0;
}
