//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>
#include <DataFile.h>


int main(int argc, char *argv[]){

    std::cout << "Reading test csv" << std::endl;

    DataFile datafile = DataFile(R"(C:\Users\17579\Development\c++\Calibration\data\visibilities.csv)", std::ios::in);
    datafile.ReadCsv();

    std::vector<std::complex<float>> vis = datafile.GetColumn<float, std::complex>("vis");

    int index = 0;
    for(auto & it: vis){
        std::cout << index << ": " << it.real() << ", " << it.imag() << std::endl;
        index++;
    }

}