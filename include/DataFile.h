//
// Created by jhoskins on 5/22/24.
//

#ifndef DATAFILE_H
#define DATAFILE_H

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <complex>
#include <charconv>

class DataFile: private std::fstream {
private:
    std::unordered_map<std::string, std::vector<std::string>> pDataFrame;

    std::vector<std::string> pColumns;

    void pParseRow(char *, const char *);
    void pParseHeader(char *, const char *);

public:
    explicit DataFile(const char *filename, const ios_base::openmode mode = ios_base::in | ios_base::out): std::fstream(filename, mode) {
        if( good() ){
            std::cout << "Opening file: " << filename<< std::endl;
        }
        else{
            std::cerr << "Error opening file: " << std::endl;
            exit(1);
        }
    }

    ~DataFile() override{close();}

    void ReadCsv(const char * delimiter = ",");
    void PrintKeys();
    void PrintDataFrame();

    static std::pair <std::string, std::string> convertComplex(const std::string &);

    // Templates
    template <typename T>
    std::vector<T> GetColumn(const char* column){

        T value;
        std::vector<T> data;

        for (auto & it : this->pDataFrame[column]) {
            const auto resp = std::from_chars(it.data(), it.data() + it.size(), value);
            data.push_back(value);
        }

        return data;

    }

    template <typename T, template <typename> class complex>
    std::vector<std::complex<T>> GetColumn(const char* column){

        T real;
        T imag;

        std::vector<std::complex<T>> data;

        for (auto & it : this->pDataFrame[column]) {

            auto z = DataFile::convertComplex(it);

            std::from_chars(z.first.data(), z.first.data() + z.first.size(), real);
            std::from_chars(z.second.data(), z.second.data() + z.second.size(), imag);
            data.push_back(std::complex<T> (real, imag));
        }

        return data;
    }

};
#endif //DATAFILE_H