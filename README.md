# Max_Clique_Prob
Parallelizing maximum clique finding problem. 

The MM (.mtx) library consists of mmio.h and mmio.c files.

Draft1.c is a C program to test the library. Reads the .mtx file with 516 nodes and 1188 edges, accesses functions from serial.c, and writes the output to the .txt file.

Understanding MM: https://math.nist.gov/MatrixMarket/formats.html#MMformat

Library: https://math.nist.gov/MatrixMarket/mmio-c.html

Dataset Info: http://networkrepository.com/bio-diseasome.php

@inproceedings{nr,
     title={The Network Data Repository with Interactive Graph Analytics and Visualization},
     author={Ryan A. Rossi and Nesreen K. Ahmed},
     booktitle={Proceedings of the Twenty-Ninth AAAI Conference on Artificial Intelligence},
     url={http://networkrepository.com},
     year={2015}
}
