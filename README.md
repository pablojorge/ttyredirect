## Description

Simple script to capture the output of a program sent to both stdout and stderr in the ORIGINAL order. 

## Problem

When you redirect both stdout and stderr to a file with the shell, you may get the output in the wrong order. This happens because tty drivers flush the buffer on newlines, but regular files flush only when the buffer becomes full.

Example:
    $ cat test.py
    import sys
    
    sys.stdout.write("stdout is a tty: %s\n" % sys.stdout.isatty())
    sys.stderr.write("stderr is a tty: %s\n" % sys.stderr.isatty())
    
    for i in range(1,3):
        sys.stdout.write("stdout %i\n" % i)
    	sys.stderr.write("stderr %i\n" % i)

    $ python test.py 
    stdout is a tty: True
    stderr is a tty: True
    stdout 1
    stderr 1
    stdout 2
    stderr 2
    $ python test.py > output.txt 2>&1
    $ cat output.txt 
    stderr is a tty: False
    stderr 1
    stderr 2
    stdout is a tty: False
    stdout 1
    stdout 2

As you can see, in the second case the program detected that stdout and stderr are not attached to a tty, and the lines are not intermixed as they were generated.

## Solution:

The solution consists of creating a temporary pseudo-tty we control and attach the program to it (using forkpty()).

    $ ./ttyredirect 
    usage:
        ./ttyredirect command [args..]
    $ ./ttyredirect python test.py 
    stdout is a tty: True
    stderr is a tty: True
    stdout 1
    stderr 1
    stdout 2
    stderr 2
    $ ./ttyredirect python test.py > output.txt
    $ cat output.txt 
    stdout is a tty: True
    stderr is a tty: True
    stdout 1
    stderr 1
    stdout 2
    stderr 2

