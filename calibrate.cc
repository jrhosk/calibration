//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>

#include <DataFile.h>
#include <AntennaSolve.h>
//
// Model rho_ij: std::vector <complex> model[N] = 1 + 0.0j
//

int main(int argc, char *argv[]){

    std::cout << "Reading test csv" << std::endl;


    DataFile visfile = DataFile("/home/mystletainn/Development/c++/calibration/data/visibilities.csv", std::ios::in);
    visfile.ReadCsv();

    std::vector<std::complex<float>> vis = visfile.GetColumn<float, std::complex>("vis");



}