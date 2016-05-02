/**
 * Program to create gblocks from fasta formatted files
 * 
 * By Derek Gasaway
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//#include <boost/program_options.hpp>

void write_80(std::ostream& os, const std::string& input) {
    std::istringstream iss (input);
    const int line_max = 80;
    char buffer[line_max + 1];
    buffer[line_max] = '\0';
    while (iss.read(buffer, line_max)) {
        os << buffer << std::endl;
    }
    if (!iss) {
        buffer[iss.gcount()] = '\0';
        os << buffer << std::endl;
    }
}

int main (int argc, char *argv[]) {
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " file\n";
        return 1;
    }
    std::string infile_name (argv[1]);
    std::ifstream infile (infile_name);
    std::string fasta;
    if (!std::getline(infile, fasta)) {
        std::cerr << "Empty file!" << std::endl;
        return 2;
    }
    if (fasta.front() != '>') {
        std::cerr << "No FASTA header!" << std::endl;
        return 2;
    }
    
    auto outfile_base_name = infile_name.substr(0, infile_name.rfind('.'));
    auto outfile_extension = infile_name.substr(infile_name.rfind('.'));
    
    std::string sequence;
    
    std::string line;
    while (std::getline(infile, line)) {
        sequence.append(line);
    }
    
    const auto sequence_length = sequence.length();
    const int gblock_max = 2000;
    const int overhang_length = 40;
    const int gblock_min = 400;
    
    //const int n_splits = sequence_length <= 0 ? 1 : (sequence_length - 1) / gblock_max + 1;
    int n_splits = 0;
    auto remaining = sequence_length;
    while (remaining > 0) {
        /*if (remaining == sequence_length) {
            if remaining <= gblock_max {
                remaining = 0;
            }
            remaining -= gblock_max;
        } else */if (remaining <= gblock_max) {
            remaining = 0;
        } else {
            remaining -= gblock_max - overhang_length;
        }
        n_splits++;
        //std::cout << remaining << std::endl;
    }
    //std::cout << "nsplits " << n_splits << std::endl;
    if (n_splits == 1) {
        auto outfile_name = outfile_base_name + '.' + std::to_string(1) + outfile_extension;
        std::ofstream outfile (outfile_name);
        outfile << fasta << std::endl;
        write_80(outfile, sequence);
        return 0;
    }
    const int split_length = sequence_length / n_splits + 1;
    //std::cout << "length " << split_length << std::endl;
    int offset = 0;
    for (int fileno = 0; fileno < n_splits; fileno++) {
        //std::cout << "hi " << fileno << std::endl;
        auto outfile_name = outfile_base_name + '.' + std::to_string(fileno) + outfile_extension;
        std::ofstream outfile (outfile_name);
        outfile << fasta << std::endl;
        write_80(outfile, sequence.substr(offset, offset + split_length));
        offset += split_length - overhang_length;
    }
    return 0;
}
