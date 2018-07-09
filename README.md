# OS-Lab
Operating System Lab is a set of course projects which helps to understand several important parts of OS, including memory management, process management, Interprocess communication, file system ...<br>
These labs simulate and achieve several mechanism in Unix.<br>
Written in C.
## File Description
Each folder represents a lab, containing 4 parts:
* bin: executable file
* code
* image: screenshots of important steps or results
* report: explain the whole lab in details
## Contents
### Lab01
* Simulate UNIX variable partition memory management to implement allocation and release management of specific memory area using the algorithm of NextFit.
### Lab02
* Using the message communication mechanism and the shared memory mechanism (using the signal light to implement synchronization and mutual exclusion between processes) in UNIX to realize data communication between two processes.
* Using function `fork` to create a child process that allows the parent and child processes to collaborate and coordinately perform a function.
### Lab03
* Further understanding, using, and mastering the system calls of files, standard subroutines of files, can use these basic file operations to complete complex file processing.
* Make comparation of several methods mentioned as follows.
#### 7 methods used in the lab to realize file copy:
* `read` and `write`
* `fread` and `fwrite`
* `fscanf` and `fprintf`
* `fgets` and `fputs`
* `fgetc` and `fputc`
* `named pipe`
* `unnamed pipe`
### Lab04
* In-depth understanding of the methods and details of socket programming by programming a practical heterogeneous socket communication program.
