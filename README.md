## Binaries

Just a bunch of executable? scripts? in c and cpp ?

## Install

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

### Source Files

the **source codes** are located in `.src` `./.completions` holds **bash
completions** for binaries

to compile them run `makeall.sh` in `.src`

---

# Documentation

## Gitalias

**gitalias** is a binary that parses the command line args into git commands for
you like an alias for git

### Aim

To be able to write a complex command in a single line eg:

```
gitalias -i -a . -c -m "New commit made" "Added new files" -b testing-branch -s testing-branch -r newReop -D 'a repo for my testing branch' -t false -o punixcorn/newRepo
```

**this command**:

- inits a repository
- add all files
- commit with the message "New commit made\n Added new files"
- creates a branch called 'testing-branch'
- switches to the testing-branch
- creates an online repository called 'newReop' with the description and type
- and pushes the files to the repo that was just created

## other features

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

## msd / umsd

mount/umount block devices

## setHdmi

set up hdmi connection in Xorg using `xrandr`

## makehere

a simple c/cpp project setup

---

### Contributors

<a href="https://github.com/punixcorn/binfiles/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=punixcorn/binfiles" />
</a>
