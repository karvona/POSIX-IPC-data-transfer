# IPC data transfer using POSIX realtime signals

## Project Overview
This project demonstrates inter-process communication by transferring Morse-encoded data using POSIX realtime signals. The program consists of a single executable that forks a child process to handle data transmission. The parent process decodes Morse-encoded signals sent by the child process and writes the decoded data to the specified output, either a file, pipe or standard output.

The primary goal of this project is educational: while POSIX signals are not inherently designed for high-volume or high-speed data transfer, this implementation provides a practical way to explore process communication, signal handling, and error management in a Linux environment.

## To build and run the code on a Linux system
First, clone the repository to your local machine:
`$ git clone <repository_url>`

Go to the repository folder
`$ cd <repository_folder>`

Create build folder
`$ mkdir build`

Build the project using the Makefile
`$ make`

Run the program with input and output file descriptors. (See example further below)
`$ ./build/main [input|-] [output|-]`


## Example Usage
In the arguments, you can specify the **input** and **output** file descriptors, or use standard input/output
#### Command-Line Arguments:
- **`<input>`**: The input file from which data will be read. You can provide a file path or use `-` to read from standard input.
- **`<output>`**: The file where the decoded output will be written. You can provide a file path or use `-` to write to standard output.

### Examples
#### Transfer data from `data.txt` to `result.txt`:

`$ ./build/main data.txt result.txt`

This will read from `data.txt`, process the data, and write the decoded result to `result.txt`.

#### Transfer data from `data.txt` to standard output:
   
`$ ./build/program data.txt -`

This will read from `data.txt` and print the decoded result to the terminal (standard output).

#### Transfer data from standard input to a file `result.txt`:

`$ ./build/program - result.txt`

This will read data from standard input and write the decoded result to `result.txt`.


## About the program and its performance

The program was developed and tested on Debian 12.7 running on VMware Workstation 16
Player on a PC equipped with a Ryzen 3600X processor.

With this setup, the program successfully passes tests with a total input size of 20.31 KB, including both short and long files and pipes, within a reasonable time frame. The average runtime is approximately 52 seconds, based on 5 tests with different seed values.

The test bench tests the program with files and pipes of up to 1024 bytes in length, meaning
that the program works as intended for outputs of 1024 bytes or shorter. When testing with
long files using the ‘-l’ parameter in the test bench, the program stops at 4096 characters,
which is the maximum block size for input characters in the code. So in conclusion, the
program currently works with files smaller than 4096 bytes


## Possible improvements
As mentioned earlier, the program currently only supports inputs up to 4096 characters,
which could be enhanced. The program could probably have been made to handle input files
of any length if there was more time for development.

To make the program perform faster/better, it would be beneficial to find an alternative way to handle the data flow control in the child process, and replace the
currently used sleep()-function with it, since sleep() is pretty unreliable, yet simple.


Also, some more characters could be added to the program, like accented letters or ÅÄÖ.
