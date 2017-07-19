#include <iostream>
#include <string>
#include <boost/program_options.hpp>


namespace po = boost::program_options;


po::variables_map* parse_options(int ac, char* av[]){
    try{
        // Define available options
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Print help message")
            ("input", po::value<std::string>(), "Set input ROOT file")
        ;

        // Parse options
        po::variables_map* vm = new po::variables_map;
        po::store(po::parse_command_line(ac, av, desc), *vm);
        po::notify(*vm);

        // Validate options
        if(vm->count("help")){
            std::cout << desc;
            return 0;
        }

        if

        return vm;
    }
    catch(std::exception& e){
        std::cerr << e.what() << "\n" << "Try --help for more information." << std::endl;
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

    // Set up lwtnn

    // Open ROOT file

    // Run event loop and add lwtnn response

    // Clean-up

    // DEBUG
    std::cout << "DEBUG" << std::endl;
    return 0;
}
