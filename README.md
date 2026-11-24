# Doubly connected edge list for watchtower coverage

This C program divides a convex polygon into regions, lists the Victorian watchtowers in each region and totals the population served by those towers. You choose where each dividing line goes by naming two edges.

The program represents the subdivision with a doubly connected edge list (DCEL). Each face has a boundary you can walk by following the next or previous half-edge. An interior edge has a twin pointing in the opposite direction on the adjacent face. These links let the program insert a dividing edge and then walk each new boundary to classify the towers.

## Build and run

The implementation and sample data are on the [`master` branch](https://github.com/JamesCondos/Doubly-Connected-Edge-Linked-List/tree/master). The following commands build the program and run the ten-tower example on Linux, including Ubuntu on WSL. They require Git and a C compiler.

```sh
git clone --branch master https://github.com/JamesCondos/Doubly-Connected-Edge-Linked-List.git
cd Doubly-Connected-Edge-Linked-List
cc -std=c11 -D_POSIX_C_SOURCE=200809L -O2 voronoi1.c -o watchtowers
head -n 1 poly_5split.txt | ./watchtowers dataset_10.csv polygon_irregular.txt report.txt
cat report.txt
```

This example takes the first split in the supplied file, `0 3`, and joins the midpoints of those two edges. The report groups the ten tower records by face and ends with these totals.

```text
Face 0 population Served: 16617
Face 1 population Served: 269
```

## Input and output

| Input | Format |
| --- | --- |
| First argument | A CSV file with a header, followed by watchtower ID, postcode, population served, contact name, x and y in that order. The supplied examples are `dataset_1.csv`, `dataset_10.csv` and `dataset_full.csv`. |
| Second argument | A convex polygon with one space-separated `x y` pair per line, in clockwise order. The program connects the last vertex back to the first. |
| Third argument | The path for the text report. The program creates or overwrites this file. |
| Standard input | One pair of space-separated edge IDs per split. Each pair selects two boundary edges of the same face. End the input at EOF. |

Edge IDs start at zero and follow the polygon's vertex order. For example, the clockwise square `(0,0), (0,2), (2,2), (2,0)` has a left edge numbered 0 and a right edge numbered 2. A split of `0 2` connects `(0,1)` to `(2,1)`. The connecting edge gets the next edge ID, followed by the two new boundary segments.

An empty input stream gives the population for the original polygon.

```sh
./watchtowers dataset_10.csv polygon_irregular.txt unsplit.txt < /dev/null
tail -n 1 unsplit.txt
```

The original face serves **16,886** people, the sum of the two regions above. The quick start uses one split because running all of `poly_5split.txt` on Ubuntu currently aborts during memory cleanup.

The [C implementation](https://github.com/JamesCondos/Doubly-Connected-Edge-Linked-List/blob/master/voronoi1.c) contains the split and point-in-face routines. The [structure definitions](https://github.com/JamesCondos/Doubly-Connected-Edge-Linked-List/blob/master/structs.h) show how the faces, half-edges and vertices fit together.
