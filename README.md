# root_lwtnn

`root_lwtnn` is an executable which can ...

- ... add a branch with the neural network response to the input file/tree.
- ... create a new ROOT file with the neural network response.

## How can I build it?

First, you have to build `lwtnn` itself. Follow the instructions of their [`README`](https://github.com/lwtnn/lwtnn). As well, you can tell the `cmake` build system of this project, where it can find `lwtnn`.

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

The folder `test/` holds a full toolchain to test the functionality of `lwtnn` and this program. First, train a simple neural network with the dataset in `tmva_class_example.root`.

```
train.py --input tmva_class_example.root
```

Then, convert the weight file `weights.h5` and the architecture `architecture.json` into a `lwtnn` config `lwtnn.json`. For this, you need to define the input variables, outputs and transformations in the file `variables.json`, which is already done in the existing file. Run the following script, to get the `lwtnn.json` file (look at the content of the file!).

```
./lwtnn_convert.sh
```

That's it! The file `lwtnn.json` holds all information needed to execute the neural network.

## Is it fast? Yes!

In order to test this, it is set up the exectuable `test/test_pythonc`. This implements the same functionality than `lwtnn` using the Python/C API. The script `test/duplicate.py` can be used to create a ROOT file with 996000 events, which is used for testing the runtime. The used Keras backend is Theano (because the environment variable `OMP_NUM_THREADS=1' makes it easy to restrict it to a single core).

The following outputs yield that `lwtnn` is about 30 times faster than the naive Python/C approach.

```
> cd <this repository>/build/

> # Python/C API
> time ./test/test_pythonc

real    1m41.416s
user    1m41.153s
sys	    0m0.194s

> # lwtnn
> time ./src/root_lwtnn \
    --input-file ../test/tmva_class_example_large.root \
    --input-tree TreeS \
    --config ../test/lwtnn.json \
    --output-file test_lwtnn.root

real    0m3.621s
user    0m3.515s
sys	    0m0.057s
```
