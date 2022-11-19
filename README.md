<!--
### these are personal scripts

### to get the set it up 

#### 1. clone the repo in your home directory

#### 2. run the file makefolderExecutable in the bin folder

  ##### this addes the folder to your path   
    ###### this is done by putting the a script in your Zsh/bash startup file eg(.zshrc,.bashrc or .bash_profile) hence you should know your default startup file


# How to use each command

#### 1. cnetwork
<p>
this is just used to check if the pc is connected to the internet
__number of args__: 1 ( number of pings wanted )
__Usage__ : 
`$ cnetwork ` this will ping the archlinux servers without stopping
`$ cnetwork 4` this will ping the archlinux servers only 4 times 
</p>

#### 2. findfile
<p>
this is just used to find a file starting from the / dir
__number of args__: 1 ( file/folder to find )( post argument )
__Usage__ : 
`$ findfile ` it will ask for the file to find
</p>

#### 2.killapp
<p>
this is just used to find a file starting from the / dir
__number of args__: 1 ( file/folder to find )( post argument )
__Usage__ : 
`$ findfile ` it will ask for the file to find
</p>

-->

## Binaries

these are binaries i wrote in C and complied and put them in a folder exported to path so i can call from anywhere

it would be better to **write them in bash** they take half the space and are easiler to edit or write but i chosse C/C++

becuase am currently learning that , they still work, all the C code does is parse the text you pass into into it and translates and arranges them accordingly

and calls the **system()** function to out them into the terminal

### Install

clone the folder to home and run the **makefolderExecutable** to automatically add the folder to path or you can do it mannually

if **makefolderExecutable** doesnt run , make it executable by 

` sudo chmod 777 makefolderExecutable `
