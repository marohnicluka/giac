# A graph theory package for Giac

This package contains a collection of graph theory commands for Giac CAS which are compatible with the corresponding Maple commands.

The commands are listed in the file "graphtheory.h". The implementation of the commands serves as a Giac interface to the **graphe** class which does all the hard work.

This library is written in C++ from scratch and has no dependencies save Giac itself. Although the focus is on simplicity, the implemented algorithms are reasonably fast.

See the comment sections in the file "graphtheory.cc" for the details of command usage.

## Compilation

To test the library, open the file "test.cc" in a text editor and enable some demos listed at the bottom of the file "graphtheory.h". Then type "make" in the command-line, followed by "./a.out".

Each demo contains several commands, which are printed on the screen and executed (displaying the results) when the corresponding function is called. If a long vector of geometric objects such as points and segments is obtained as the result of running a command, it represents a drawing. To view it, you must copy it to an active Xcas session.

To test custom commands, you need to code them in the procedure main in "test.cc" (see the Demo section at the bottom of "graphtheory.cc" for a practical reference).