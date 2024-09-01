/*
 *******************************************************************
 * rewrites
 * 1 syscall_mount_dev = pure syscalls, fails because of portability
 * 2 execv_dev = execv* syscall, fails because sudo has IO errors
 *
 *******************************************************************

 Add / Fixes
 *******************************************************************
 1. --mount, to mount by default at /mnt
 */

#include <format>
#include <print>
// mount syscall
#include <unistd.h>

#include <cstdlib>

// if syscall mount is used ( unstable and not portable )
#ifdef syscall_mount_dev
#include <errno.h>
#include <linux/magic.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#endif

// C++ libs
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "../engine/engine.hpp"
#define null NULL
#define check_null(var, msg) \
    if (var == null) {       \
        ERR(msg);            \
    }

// -p <dir> -d <dev.paramSet>
// dir dev.paramSet
// -p dir <dev.paramSet>
// -d <dev.paramSet> <dir>

struct device_t {
    bool paramSet{false};
    std::string data;
    device_t(const bool isParmaSet = false, std::string Data = "")
        : paramSet(isParmaSet), data(Data) {};
    device_t(device_t &&dev) = default;
};

struct dir_t : public device_t {
    bool umount_t{false};
    std::string umount_dir_t;
};

class Parser : public Engine {
   private:
    int Argc;
    char **Argv;

   public:
    Parser(int argc, char **argv);

    // if passed in as device_name instead of /dev/device_name, fix it
    void make_device_path_absolute(std::string &device_name) {
        if (device_name.find("/dev/") == std::string::npos) {
            std::string temp{device_name};
            device_name.clear();
            device_name = std::format("/dev/{}", temp);
        };
    }

    //  returns true if both disk and place where found
    bool setDeviceAndDir(std::vector<std::string> params, std::string &place,
                         std::string &disk) {
        if (params.size() != 2) return false;

        // checks if the input is /dev/device_name or just device_name
        std::string dev_path{"/dev/"};
        if (params[0].find("/dev") != std::string::npos ||
            params[1].find("/dev") != std::string::npos) {
            dev_path.clear();
            dev_path = "";
        }

        // check which is /dev/device
        if (std::filesystem::exists(std::format("{}{}", dev_path, params[0]))) {
            disk = params[0];
            place = params[1];
        } else if (std::filesystem::exists(
                       std::format("{}{}", dev_path, params[1]))) {
            disk = params[1];
            place = params[0];
        } else {
            return false;
        }

        if (!std::filesystem::exists(place)) {
            return false;
        }
        return true;
    };

#ifdef syscall_mount_dev
    // find the filesystem type of a device
    const char *find_file_system_type(long f_type) {
        switch (f_type) {
            case TMPFS_MAGIC:
                return "tmpfs";
            case NFS_SUPER_MAGIC:
                return "nfs";
            case V9FS_MAGIC:
                return "9p";
            case RAMFS_MAGIC:
                return "ramfs";
            case BTRFS_SUPER_MAGIC:
                return "btrfs";
            case XFS_SUPER_MAGIC:
                return "xfs";
            case FUSE_SUPER_MAGIC:
                return "fuse";
            case EXT2_SUPER_MAGIC:
                return "ext4";
            default:
                return "unknown";
        }
    };
#endif

    // parse Argc and Argv
    void parse() {
        device_t dev;
        dir_t dir{};
        std::vector<std::string> params{};
        int nonParamCount{0};
        switch (Argc) {
            case 2:
                if (!(isOptionFound(std::string(Argv[1]), "--help") ||
                      isOptionFound(std::string(Argv[1]), "--umount"))) {
                    ERR("Invalid arguments passed", 1);
                }
            default:
                int i{1};
                while (i < Argc) {
                    if (isOptionFound(Argv[i], "--help")) {
                        print(
                            "msd:  tool for mounting devices\n"
                            "Usage: msd [ options... ] \n"
                            "<device> <dir>               mount <device> at "
                            "<dir>\n"
                            "-h,--help                    print this message\n"
                            "-p,--place <dir>             place to mount"
                            "<dir>\n"
                            "-d,--device <device>         device to mount\n"
                            "-u,--umount <dir>            unmount device from "
                            "mount point <dir>\n");

                        exit(0);
                    } else if (isOptionFound(Argv[i], "--place")) {
                        ++i;
                        check_null(Argv[i],
                                   "--place needs an argument, where to mount "
                                   "the device");
                        validate_option_arg(
                            Argv[i],
                            "--place needs an argument, where to mount "
                            "the device");
                        dir.paramSet = true;
                        dir.data = std::string(Argv[i] == null ? "" : Argv[i]);

                    } else if (isOptionFound(Argv[i], "--device")) {
                        ++i;
                        check_null(Argv[i],
                                   "--device needs an argument, which special "
                                   "file (device) to mount");
                        validate_option_arg(Argv[i],
                                            "--device needs an argument, which "
                                            "special file (device) to mount");
                        dev.paramSet = true;
                        dev.data = std::string(Argv[i] == null ? "" : Argv[i]);
                    } else if (isOptionFound(Argv[i], "--umount")) {
                        ++i;
                        check_null(Argv[i],
                                   "--umount needs an argument, which "
                                   "dir to un mount\n");
                        validate_option_arg(Argv[i],
                                            "--umount needs an argument, which "
                                            "dir to un mount\n");
                        dir.umount_t = true;
                        dir.umount_dir_t =
                            std::string(Argv[i] == null ? "" : Argv[i]);
                    } else {
                        params.push_back(Argv[i]);
                        nonParamCount++;
                    }
                    i++;
                };  // while
        };

        if (dir.umount_t && dir.umount_dir_t.length() > 0) {
            if (!std::filesystem::exists(dir.umount_dir_t)) {
                ERR(std::format("could not resolve path for --umount {}",
                                dir.umount_dir_t));
            }
#ifdef umount_syscall_dev
            setuid(0);
            int umount_t = umount2(dir.umount_dir_t.c_str(), MNT_FORCE);
            if (umount_t == -1) {
                ERR(std::format("Error occured with umount\n{}\n",
                                strerror(errno)));
            }
#endif
            std::string temp =
                std::format("sudo umount {}  -f -l -v", dir.umount_dir_t);

            std::system(temp.c_str());
#ifdef execv_dev
            if (fork() == 0) {
                char *const argv[] = {"bash", "-c", (char *const)temp.c_str(),
                                      null};
                int execvi = execvp("bash", argv);

                if (execvi == -1) {
                    ERR(std::format("ERR OCCURED\n{}\n", strerror(errno)));
                    exit(1);
                }
            }
#endif
            std::print("{} umounted\n", dir.umount_dir_t);
            if ((nonParamCount < 2) && (!dev.paramSet && !dir.paramSet)) {
                exit(1);
            }
        }

        switch (nonParamCount) {
            case 0:
                if (dev.paramSet && dir.paramSet) {
                    break;
                } else {
                    ERR("Invalid args! at dev.paramSet && dir "
                        "nonParamCount == "
                        "0");
                }
                break;
            case 1:
                if (dev.paramSet) {
                    dir.data = params.size() > 0 ? params[0] : "";
                } else if (dir.paramSet) {
                    dev.data = params.size() > 0 ? params[0] : "";
                } else {
                    ERR("Invalid args! at dev.paramSet || dir "
                        "nonParamCount == "
                        "1");
                }
                break;
            case 2:
                setDeviceAndDir(params, dir.data, dev.data);
                break;
            default:
                ERR("program terminated");
        }

        make_device_path_absolute(dev.data);

        if (dir.data.find("/dev") == 0) {
            ERR("dir cannot be /dev/device_name\n");
        }

        if (!std::filesystem::exists(dir.data)) {
            ERR(std::format("DIR {} not found try using absolute path\n",
                            dir.data));
        }

        if (!std::filesystem::exists(dev.data)) {
            ERR(
                std::format("device file {} not found\n"
                            "check mount points\n",
                            dir.data));
        }

#ifdef syscall_mount_dev
        // mount syscall
        struct statfs s;
        statfs(dev.data.c_str(), &s);
        if (mount(dev.data.c_str(), dir.data.c_str(),
                  find_file_system_type(s.f_type), MS_SYNCHRONOUS, null))
            ERR("mount failed");
#endif

        std::string temp =
            std::format("sudo mount  -o umask=000 {} {}", dev.data, dir.data);
        std::system(temp.c_str());
#ifdef execv_dev
        char *const argv[] = {(char *const)"bash", (char *const)"-c",
                              (char *const)temp.c_str(), null};

        if (fork() == 0) {
            int execvi = execvp("bash", argv);
            if (execvi == -1) {
                ERR(std::format("ERR OCCURED\n{}\n", strerror(errno)));
                exit(1);
            }
        } else {
            std::print("{} mounted at {}\n", dev.data, dir.data);
        }
#endif
    }  // parse
};

Parser::Parser(int argc, char **argv) : Engine(argc, argv) {
    assert(argv != null);
    assert(argc > 0);
    Argc = argc;
    Argv = argv;
};

int main(int argc, char *argv[], char **envp) {
    try {
        Parser e(argc, argv);
        e.parse();
    } catch (const std::logic_error e) {
        std::print("Parser failed\n info {}\n", e.what());
        throw std::logic_error(e);
    } catch (const std::bad_cast e) {
        std::print("bad cast\n");
        throw std::bad_cast(e);
    }
    return 0;
}
