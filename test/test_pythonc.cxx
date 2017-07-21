#include <iostream>
#include "Python.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include "TFile.h"
#include "TTree.h"


void PyRunString(const char* code,
        PyObject* global_ns, PyObject* local_ns,
        int start = Py_single_input)
{
    PyObject* err = PyRun_String(code, start, global_ns, local_ns);
    if (!err){
        std::cout << "Executing Python code failed: " << code << std::endl;
        PyErr_Print();
    }
}


int main(void){
    // Configuration (hardcoded)
    TString input_filename = "../test/tmva_class_example_large.root";
    TString input_treename = "TreeS";
    TString output_filename = "test_pythonc.root";
    TString output_treename = "response";
    std::vector<TString> varnames = {"var1", "var2", "var3", "var4"};

    // Open input file and set branches
    TFile* input_file = new TFile(input_filename, "READ");
    TTree* input_tree = (TTree*) input_file->Get(input_treename);
    float vars[4];
    for(size_t i=0; i<varnames.size(); i++)
        input_tree->SetBranchAddress(varnames[i], vars+i);

    // Open output file and book branches
    TFile* output_file = new TFile(output_filename, "RECREATE");
    TTree* output_tree = new TTree(output_treename, output_treename);
    float response[2];
    output_tree->Branch("signal", response, "signal/F");
    output_tree->Branch("background", response+1, "background/F");

    // Start Python interpreter and set up namespaces
    Py_Initialize();
    PyObject* local_ns = PyDict_New();
    PyObject* main_module = PyImport_AddModule("__main__");
    PyObject* global_ns = PyModule_GetDict(main_module);

    // Load Keras model

    // Set sys.argv manually because its not populated by default in this environment.
    // Otherwise, Tensorflow fails during import.
    PyRunString("import sys", global_ns, local_ns);
    PyRunString("sys.argv = ['']", global_ns, local_ns);

    PyRunString("from keras.models import model_from_json", global_ns, local_ns);
    PyRunString("model = model_from_json(open('../test/architecture.json').read())",
            global_ns, local_ns);
    PyRunString("model.load_weights('../test/weights.h5')",
            global_ns, local_ns);

    // Set up numpy arrays
    _import_array();

    npy_intp dims_vars[2] = {(npy_intp)1, (npy_intp)4};
    PyArrayObject* py_vars = (PyArrayObject*)PyArray_SimpleNewFromData(2,
            dims_vars, NPY_FLOAT32, (void*)vars);
    PyDict_SetItemString(local_ns, "vars", (PyObject*)py_vars);

    npy_intp dims_response[2] = {(npy_intp)1, (npy_intp)2};
    PyArrayObject* py_response = (PyArrayObject*)PyArray_SimpleNewFromData(2,
            dims_response, NPY_FLOAT32, (void*)response);
    PyDict_SetItemString(local_ns, "response", (PyObject*)py_response);

    // Run event loop
    for(size_t ievent=0; ievent<input_tree->GetEntries(); ievent++){
        input_tree->GetEntry(ievent);
        PyRunString("x = model.predict(vars)", global_ns, local_ns);
        PyRunString("response[0:2] = x[0:2]", global_ns, local_ns);
        output_tree->Fill();
    }
    output_file->Write();

    // Clean-up
    input_file->Close();
    output_file->Close();
}
