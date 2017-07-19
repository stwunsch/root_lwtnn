#include <iostream>
#include <fstream>
#include <string>
#include "boost/program_options.hpp"
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/parse_json.hh"
#include "TFile.h"
#include "TTree.h"


namespace po = boost::program_options;


po::variables_map* parse_options(int ac, char* av[]){
    try{
        // Define available options
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Print help message")
            ("verbose", "Make verbose output")
            ("input-file", po::value<std::string>(),
                "Input ROOT file")
            ("input-tree", po::value<std::string>(),
                "Tree in input ROOT file")
            ("config", po::value<std::string>(),
                "lwtnn config file")
            ("output-file", po::value<std::string>(),
                "Output ROOT file with neural network response")
            ("output-tree", po::value<std::string>()->default_value("response"),
                "Tree in output ROOT file with neural network response")
            ("filemode", po::value<std::string>()->default_value("RECREATE"),
                "Filemode of output ROOT file")
        ;

        // Parse options
        po::variables_map* vm = new po::variables_map;
        po::store(po::parse_command_line(ac, av, desc), *vm);
        po::notify(*vm);

        // Validate options

        // Check verbose flag
        bool verbose;
        if(vm->count("verbose")) verbose = true;
        else verbose = false;

        // Help
        if(vm->count("help")){
            std::cout << desc;
            return 0;
        }

        // Input file
        if(!vm->count("input-file")){
            std::cerr << "No input ROOT file given." << std::endl;
            return 0;
        }

        // lwtnn JSON file
        if(!vm->count("config")){
            std::cerr << "No lwtnn config file given." << std::endl;
            return 0;
        }
        else{
            if(verbose)
                std::cout << "lwtnn config file: "
                          << (*vm)["config"].as<std::string>() << std::endl;
        }

        // Tree name
        if(!vm->count("input-tree")){
            std::cerr << "No tree in input ROOT file defined." << std::endl;
            return 0;
        }
        else{
            if(verbose)
                std::cout << "Input file (tree path): "
                          << (*vm)["input-file"].as<std::string>()
                          << " (" << (*vm)["input-tree"].as<std::string>() << ")"
                          << std::endl;
        }

        // Output file
        if(!vm->count("output-file")){
            std::cerr << "No output ROOT file given." << std::endl;
            return 0;
        }
        else{
            if(verbose)
                std::cout << "Output file (file mode): "
                          << (*vm)["output-file"].as<std::string>()
                          << " (" << (*vm)["filemode"].as<std::string>() << ")"
                          << std::endl;
        }

        return vm;
    }
    catch(std::exception& e){
        std::cerr << e.what() << "\n" << "Try --help for more information."
                  << std::endl;
        return 0;
    }
    catch(...){
        std::cerr << "Exception of unknown type!\n";
        return 0;
    }
}


int main(int ac, char* av[]){
    // Parse options
    po::variables_map* vm = parse_options(ac, av);
    if(vm==0) return 1;

    bool verbose;
    if(vm->count("verbose")) verbose = true;
    else verbose = false;

    // Set up lwtnn
    std::ifstream config_file((*vm)["config"].as<std::string>().c_str());
    lwt::JSONConfig config = lwt::parse_json(config_file);
    lwt::LightweightNeuralNetwork model(
        config.inputs,
        config.layers,
        config.outputs);

    lwt::ValueMap inputs;
    for(size_t n=0; n < config.inputs.size(); n++)
            inputs[config.inputs.at(n).name] = 0.0;

    // Open ROOT input file, get tree and register branches
    TFile* file_input = new TFile((*vm)["input-file"].as<std::string>().c_str(),
            "READ");
    if(file_input==0){
        std::cerr << "Can not open input ROOT file." << std::endl;
        return 1;
    }

    TTree* tree_input = static_cast<TTree*>(
            file_input->Get((*vm)["input-tree"].as<std::string>().c_str()));
    if(tree_input==0){
        std::cerr << "Can not find tree in input ROOT file." << std::endl;
        return 1;
    }

    float input_values[config.inputs.size()];
    for(size_t n=0; n < config.inputs.size(); n++){
        TString name = config.inputs.at(n).name;
        Int_t ret = tree_input->SetBranchAddress(name, input_values+n);
        if(ret<0){
            std::cerr << "Failed to set branch address for this branch: "
                      << name << std::endl;
            return 1;
        }
    }

    // Create ROOT output file, set up tree and branches
    TFile* file_output = new TFile((*vm)["output-file"].as<std::string>().c_str(),
            (*vm)["filemode"].as<std::string>().c_str());
    if(file_output==0){
        std::cerr << "Can not open output ROOT file." << std::endl;
        return 1;
    }

    TString tree_output_name = (*vm)["output-tree"].as<std::string>().c_str();
    TTree* tree_output = new TTree(tree_output_name, tree_output_name);
    double outputs[config.outputs.size()];
    for(size_t n=0; n < config.outputs.size(); n++){
        TString name = config.outputs[n];
        tree_output->Branch(name, outputs+n, name+"/D");
    }

    // Run event loop and add lwtnn response to output file
    if(verbose) std::cout << "Number of entries in tree: "
                          << tree_input->GetEntries() << std::endl;
    for(UInt_t ievent=0; ievent<tree_input->GetEntries(); ievent++){
        tree_input->GetEntry(ievent);
        for(size_t n=0; n<config.inputs.size(); n++)
            inputs[config.inputs.at(n).name] = input_values[n];
        lwt::ValueMap vmap = model.compute(inputs);
        for(size_t n=0; n<config.outputs.size(); n++)
            *(outputs+n) = vmap[config.outputs[n]];

        tree_output->Fill();
    }
    file_output->Write();

    // Clean-up
    file_input->Close();
    file_output->Close();

    return 0;
}
