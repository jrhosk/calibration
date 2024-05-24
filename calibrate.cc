//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>
#include <DataFile.h>

//
// Model rho_ij: std::vector <complex> model[N] = 1 + 0.0j
//

int main(int argc, char *argv[]){

    std::cout << "Reading test csv" << std::endl;


    DataFile datafile = DataFile("/home/mystletainn/Development/c++/calibration/data/visibilities.csv", std::ios::in);
    datafile.ReadCsv();

    std::vector<std::complex<float>> vis = datafile.GetColumn<float, std::complex>("vis");

    std::vector<float> antA = datafile.GetColumn<float>("ant_a");
    std::vector<float> antB = datafile.GetColumn<float>("ant_b");

    int index = 0;
    for(auto & it: vis){
        std::cout << antA[index] << ", " <<  antB[index] << ": " << it.real() << ", " << it.imag() << std::endl;
        index++;
    }


}