# A graph theory package for Giac

This repository contains a collection of graph theory commands for Giac CAS which are compatible with the corresponding Maple commands.

The commands are listed in **graphtheory.h**. The implementation of the commands serves as a Giac interface to the **graphe** class in which all the hard work is done.

This library is written in C++ from scratch and has no dependencies save Giac itself. The implemented algorithms are reasonably fast.

See **graphtheory.cc** for the details of command usage.