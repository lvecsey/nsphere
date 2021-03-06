Show the randomness of a given file visually with OpenGL (snow globe)

```console
./autogen.sh
mkdir build ; cd build
../configure --prefix=$HOME/nsphere-git
make check
make install
```

```console
src/nsphere /path/to/datafile 64000
```

The above example will plot with 64000 points, and rotate the sphere for you. By default it will plot 8000 points.

You can also run nsphere-ppm to output a single out1.ppm image to disk. This program has many optional args.

```console
src/ppm-out/nsphere-ppm -3 /path/to/datafile
display ./out1.ppm
```

See 'original.txt' for the file that nsphere-ppm and this entire project is based on.

[Sample image](https://phrasep.com/~lvecsey/software/nsphere/output_nsphere.png)

![Image of output](https://phrasep.com/~lvecsey/software/nsphere/output_nsphere.png)
