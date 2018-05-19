# graph-theory
**A graph theory package for Giac computer algebra system**

This repository contains a collection of graph theory commands for Giac which are compatible with the corresponding Maple commands.

The commands are defined in **graphtheory.h**. The implementation of the commands serves as a Giac interface to the **graphe** class in which all the hard work is done.

This library is written from scratch and has no dependencies save Giac itself. The implemented algorithms are reasonably fast, making the commands capable of handling moderately sized graphs (with several hundred vertices or less).

## Features
### Constructing graphs
- loading graphs from DOT files
- creating new graphs from lists of vertices and edges or from adjacency matrix
- various predefined special graphs (e.g. Petersen graph)
- various commands with graph templates (e.g. Sierpinski graphs)
- Cartesian and tensor products
### Modifying graphs
- adding and removing vertices or edges/arcs
- contracting and subdividing edges
- adding and removing attributes attached to vertices or edges (or to the graph itself), e.g. edge weights or vertex colors, supporting custom user tags