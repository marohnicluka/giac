# A graph theory package for Giac

This package contains a collection of graph theory commands for Giac CAS which are compatible with the corresponding Maple commands.

The commands are listed in *graphtheory.h*. The implementation of the commands serves as a Giac interface to the *graphe* class in which all the hard work is done.

This library is written in C++ from scratch and has no dependencies save Giac itself. The implemented algorithms are reasonably fast.

See the comment sections in *graphtheory.cc* for the details of command usage.

## Compilation

To test the library, open the file *test.cc* in a text editor and enable some demos listed at the bottom of the file *graphtheory.h*. Then type "make" in the command-line, followed by "./a.out".

To test custom commands, you need to code them in the procedure main in *test.cc* (see the Demo section at the bottom of *graphtheory.cc* for a practical reference).