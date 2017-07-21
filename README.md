# root_lwtnn

`root_lwtnn` is an executable which ...

- ... adds a branch with the neural network response to the input file/tree.
- ... creates a new ROOT file with the neural network response.

## How can I build it?

First, you have to build `lwtnn` itself. Follow the instructions of their [`README`](https://github.com/lwtnn/lwtnn).

```
cd <this repository>
git clone https://github.com/lwtnn/lwtnn
cd lwtnn
# build lwtnn ;)
```

The project is build using `cmake`, therefore run the following commands.

```
cd <this repository>
mkdir build
cd build
cmake ..
make
```

## How can I train a model with Keras and load it with this program?

## Is it fast? Yes!

In order to test this, it is set up the exectuable `test/test_pythonc`. This implements the same functionality than `lwtnn` using the Python/C API. The script `test/duplicate.py` can be used to create a ROOT file with 996000 events, which is used for testing the runtime. The used Keras backend is Theano (because the environment variable `OMP_NUM_THREADS=1' makes it easy to restrict it to a single core).

The following outputs yield that `lwtnn` is about 30 times faster than the naive Python/C approach.

```
> cd <this repository>/build/

> # Python/C API
> time ./test/test_pythonc

real	1m41.416s
user	1m41.153s
sys	    0m0.194s

> # lwtnn
> time ./src/root_lwtnn \
    --input-file ../test/tmva_class_example_large.root \
    --input-tree TreeS \
    --config ../test/lwtnn.json \
    --output-file test_lwtnn.root

real	0m3.621s
user	0m3.515s
sys	    0m0.057s
```
