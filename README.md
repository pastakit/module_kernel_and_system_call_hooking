# Module kernel and System call hooking
This project will cover the some topics about linux kernel module, file system and device in linux, communication between user space and kernel space

## Section 1: Module kernel linux 
Write a linux model to gen random numbers, this module will create a charater device to allow to processes in user space can <em> open </em> and <em> read </em> these numbers (check out <em> random_number_driver </em> folder)

## Section 2: Hooking to system call
 - Hooking to open syscall => write to dmesg the names of the processes that have a file open and this file name
 - Hooking to write syscall  => write to dmesg the names of the processes that have a file write open and the number of bytes that written

# Usage
### Section 1

- Put "random_number_driver.c" and "Makefile" in the same folder
- Execute  <em> ``makefile`` </em>
- Install .ko file: <em>``insmod random_number_driver.ko``</em>
- Finally, read random numbers: <em> ``cat /dev/random_number_driver`` </em>, checkout in dmesg 

    Also, we can test this driver using "driver_testing.c" and gcc:
 - Execute: <em>``gcc driver_testing.c -o driver_testing ``</em>
 - Execute: <em>``./driver_testing``</em>
 - ==> See random numbers within terminal, also check in dmesg for verifying

### Section 2
Hooking open syscall
 - Go to <em> hooking_open </em> folder
 - Go to <em> hooking_open </em> folder
 - Build with <em> make </em> command 
 - Install : <em>``insmod open_hooking.ko``</em>
 - ==> Testing: run any process that using open syscall, see the files and the name of processes

 Hooking write syscall
 - Similar to open syscall 
 - Using <em> hooking_write </em> folder instead of  <em> hooking_open </em>
 - Install : <em>``insmod write_hooking.ko``</em> instead of <em>``insmod open_hooking.ko``</em>

# LICENSE
VNU-HCMUS - https://www.hcmus.edu.vn
