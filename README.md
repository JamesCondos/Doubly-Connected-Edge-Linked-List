# Doubly-Connected-Edge-Linked-List
A DCELL (Doubly Connected Edged LinkedList) created in the context of checking the population in a specific area (WatchTower data)

These data come from properties of Victorian postcodes and a simple initial polygon. A user will be able to provide modifications to this polygon and receive information about what points land in each location.

Your implementation will receive three filenames as arguments and will build this data structure by reading from the first two files. The first file will contain a list of comma-separated values representing the location of a set of watch-towers and associated data for each of these watch towers (such as the population served) which you will read into a dynamically allocated array of pointers to a simple data structure containing this information. 

The second file will contain a list of points, one per line, of the initial polygon, each of the coordinates separated by space. You may assume this polygon is always convex.

e.g. if the original polygon was the list of points [0,0], [0,2], [2,2], [2,0], this would generate 4 edges, [0,0] → [0,2] (edge 0), [0,2] → [2,2] (edge 1), [2,2] → [2,0] (edge 2) and [2,0] → [0,0] (edge 3).


On standard input, you will receive a list of pairs of integers. Each pair represents a split which should occur in the polygon this split should bisect the two edges, beginning in the middle of the first edge and finishing at the middle of the second edge.

E.g. for the polygon above, the input 2, 3 would generate two additional points. The first point bisects edge 2 at position (2, 1) and the second points bisects edge 3 at position (0, 1). Those two new points are connected with an additional edge. This new edge is also numbered sequentially as Edge 4. The starting and ending edge are now split and 2 new edges are added (edge 5 and edge 6). Further details on the numbering of these splits are given in the Implementation Details section, but the result of this is shown here:



This generates two areas, shown here as Face 0 and Face 1


The third file will be used to output the result of applying all splits. After all the splits have been processed, you'll output the information, first with the face, followed by each point of interest which lies inside that face. Finally you'll output the total population in each face.
