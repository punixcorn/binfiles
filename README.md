# Binaries

Just a bunch of executable? scripts? in c and cpp ?

### INSTALL

added an installer that will **add the folder to path and source the bashfiles
for completions** <br/> **NB** : installer script works if this is cloned in
your home folder and you use `zsh` or `bash` <br/>

```bash
cd ~
git clone https://github.com/punixcorn/binfiles
cd ~/binfiles
chmod a+x *
./install.sh
```

or mannually just add the folder to path and source all files in `.completions`

---

### BUILD

**build script** can be found in `./src/build.sh`

##### Deps:

- cmake
- make
- libfmt

```bash
cd ~
git clone https://github.com/punixcorn/binfiles
cd ~/binfiles/src
chmod a+x ./build.sh
./build.sh
```

---

### Source Files

- the **source codes** are located in `.src`
- `.completions` holds **bash completions** for binaries

---

# Documentation

<h2 align="center">Gitalias</h2>

**gitalias** is a binary that parses the command line args into git commands for
you like an alias for git

To be able to write a bunch of git commands in a single short line

#### EXAMPLE :

```bash
gitalias -i -a . -c -m "New commit made" "Added new files" -b testing-branch -s testing-branch -r newReop -D 'a repo for my testing branch' -t false -o punixcorn/newRepo
```

**DOES:**

- inits a repository
- add all files
- commit with the message "New commit made\n Added new files"
- creates a branch called 'testing-branch'
- switches to the testing-branch
- creates an online repository called 'newReop' with the description and type
- and pushes the files to the repo that was just created

### MORE INFO

- if you don't **init** a repo, it will automatically ask to do so
- you can commit without a message , an automatic message will be made
- you can ignore adding files, it will automatically add files, hence:

```
gitalias -c // this will add all files, and commit with a message 'commit made'
```

- `-g/--git` command to add other git commands that are not found by default,
  **eg:**\
  `gitalias -g "add ." "commit -m 'foo'"` -> `git add . && git commit -m 'foo'`
  no need to repeat the `-g`, spaces will be translated into a `-g`

- `--message/-m` spaces will be translated into a newline, **eg**:\
  `ga -c -m 'New feature' 'added some files' 'most in c'` will be translated
  into

```txt
New feature
added some files
most in c
```

- `-C/--Clone` Cloning just requires the username and folder , if you want to
  use ssh you can invoke with `-R/--Request` ,**eg:**\
  `ga -C punixcorn/foo` -> **clone repo, with https**\
  `ga -C punixcorn/foo -R ssh` -> **clone repo using ssh**\
  `ga -C punixcorn/foo -R https` -> **clone repo using https**

- `-G/--Grab` clones a folder from a **Github** repository using `svn`, all you
  need to attach the link to file/folder in the Github repo
- `-r/--repo & -D/--Des & -t/--type` creates an online repository using `curl`\
  requires github token in /usr/githubToken, **eg:**\
  `ga -r newrepo -D 'a New repository' -t true`\
  `--type/-t` is **true/false** for making the repo private\
  **Translates to:**
  ```
  You are about to create an online repository with the following data :
   Name of repository : newrepo
   Description : a new repository
   Mode: Private
   continue with creation of the online repository [y,N] :
  ```

<h2 align="center">msd</h2>
mount

```bash
msd /mnt sda1
```

### DOES

- mounts `/dev/sda1` at `/mnt`
- but with extra stuff on it

### MORE INFO

- `-lsblk` show block devices before running command

```bash
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
nvme0n1     259:0    0 238.5G  0 disk
├─nvme0n1p1 259:1    0   512M  0 part /boot
├─nvme0n1p2 259:2    0     9G  0 part [SWAP]
├─nvme0n1p3 259:3    0 207.7G  0 part /
└─nvme0n1p6 259:4    0  21.3G  0 part
mount:
```

    mount: /mnt nvme0n1p6

- **mount point** and **block device** can be in any other
- `msd /mnt sda1` **is the same as** `msd sda1 /mnt`
- `msd /mnt /dev/sda1` is also valid

<h2 align="center">umsd</h2>

```bash
umsd /mnt
```

- umounts filesystem at /mnt

##### will be merged into msd

---

<h2 align="center">setHdmi</h2>

set up hdmi connection in Xorg using `xrandr`

---

<h2 align="center">makehere</h2>

A very simple **C/C++** project setup

#### EXAMPLE:

```bash
makehere -l cxx -p helloworld -f Main.cpp -c cmake
```

#### DOES:

- Creates a folder called helloworld
- creates a file Main.cpp in that folder
- Creates a CMakeLists.txt and Makefile to compile the cxx file

### MORE INFO

- `-l --language`
  - language of the project only `c` or `cpp`
- `-p --project`
  - name of the project
- `-c  --compiler`
  - compiler build Script `make` or `cmake`
  - default: `make`
- `-f  --filename`
  - custom file name default: `main`
- `-std --standard`
  - set a standard `11 14 17 20 23 2a 2x 2b`
  - default: `23`
- `-t --template`
  - copy template files
  - basically copy premade `Makefile CMakeLists.txt Cpp files C files`
  - `Makefile` from `~/.Makefile`
  - `CMakeLists.txt` from `~/.CMakeLists.txt`
  - `cpp file` from `~/.template.cpp`
  - `c file` from `~/.template.c`

##### NB:

- `-l` and `-p` is compulsory, Everythin else has a default

- using `--template / -t` requires you to mannually to edit them

---

### Contributors

<a href="https://github.com/punixcorn/binfiles/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=punixcorn/binfiles" />
</a>
