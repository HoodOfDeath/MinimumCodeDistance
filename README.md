# MinimumCodeDistance
From code parameters (namely dimensions) create all possible binary block codes with such a parameters by creating all possible systematic codes, and outputs the number of codes with different minimal code distances.

This repository is meant to be a proof of me growing as a programmer. If you try to run this code with a parameters of n=10 k=5 (from first commit after initial one), you will probably run out of memory. The reason is, I was not teached how to properly write C++ code in my university.

The overall goal of this project is to make this code usable, and, I hope, useful to somebody.

The baseline for iteration as of now is looks like this:

1) Fix memory leaks
2) Optimise memory usage
3) Multithreading the process