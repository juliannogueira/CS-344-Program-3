Author: Julian Torres
Course: CS-344
Assignment: Program-3

Notes

    The behavior of SIGTSTP is altered when running the program from within
    make and valgrind. Therefore, it is recommended to test SIGTSTP
    functionality by Method 2.

    All other shell functionality is normal within
    make and valgrind.

To compile the code

    Method 1

        make

To run the program

    Method 1

        make run

    Method 2

        ./smallsh

To check for memory leaks

    Method 1

        make check

    Method 2

        valgrind --leak-check=yes ./smallsh
