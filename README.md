## Binaries

these are binaries i wrote in C and bash and complied and put them in a folder exported to path so i can call from anywhere

it would be better to **write them in bash** , bash scripts take half the space and they are easier to edit or write but i chosse **C/C++**

###### as of now not all are written in C

because am currently learning that , they still work, all the C code does is parse the text you pass into into it and translates and arranges them accordingly

and calls the **system()** function to out them into the terminal, nothing big or complicated

###### will soonbe rewritten to use the syscalls instead of calling **system()**

###### found out mostly there is no reason to rewrite them in syscalls since the linux functions are just wrappers

### Install

clone the folder to home and run the **makefolderExecutable** (in .src) to automatically add the folder to path or you can do it mannually

### Errors

most **errors** are from the bashfiles not having the right permissions

if **makefolderExecutable** doesnt run , make it executable by
`sudo chmod 777 makefolderExecutable`

if **makehere** doesnt run , make it an executable by
`sudo chmod 777 makehere`

### Source Files

the **source codes** are located in .src
