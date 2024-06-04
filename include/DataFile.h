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

public:
    std::unordered_map<std::string, std::vector<std::string>> dataframe;

    std::vector<std::string> pColumns;

    void ParseRow(char *, const char *);
    void ParseHeader(char *, const char *);

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
    void PrintKeys() const;
    void PrintDataFrame() const;

    static std::pair <std::string, std::string> ConvertComplex(const std::string &);

    // Templates

//#if __cpp_lib_to_chars >= 201611L
    template <typename T>
    std::vector<T> GetColumn(const char* column){

        T value;
        std::vector<T> data;

        for (auto & it : this->dataframe[column]) {
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

        for (auto & it : this->dataframe[column]) {

            auto z = DataFile::ConvertComplex(it);

            std::from_chars(z.first.data(), z.first.data() + z.first.size(), real);
            std::from_chars(z.second.data(), z.second.data() + z.second.size(), imag);
            data.push_back(std::complex<T> (real, imag));
        }

        return data;
    }
//#endif
/**
#if __cpp_lib_to_chars < 201611L

/**
    std::vector<int> GetColumn(const char* column) {

        int value;
        std::vector<int> data;

        for (auto & it : this->dataframe[column]) {
            value = strtof(it.data(), nullptr);

            data.push_back(value);
        }

        return data;

    }
**/
    /**
    std::vector<std::complex<float>> GetColumn(const char* column){

        float real;
        float imag;

        std::vector<std::complex<float>> data;

        for (auto & it : this->dataframe[column]) {

            auto z = DataFile::ConvertComplex(it);

            real = std::strtof(z.first.data(), nullptr);
            imag = std::strtof(z.second.data(), nullptr);
            data.push_back(std::complex<float> (real, imag));
        }

        return data;
    }

#endif
**/

};
#endif //DATAFILE_H