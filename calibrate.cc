//
// Created by jhoskins on 5/22/24.
//
#include <iostream>
#include <complex>
#include <DataFile.h>

#include <charconv>
#include <iomanip>
#include <iostream>


int main(int argc, char *argv[]){
    /**
    std::cout << "Reading test csv" << std::endl;


    DataFile datafile = DataFile(R"(C:\Users\17579\Development\c++\Calibration\data\visibilities.csv)", std::ios::in);
    datafile.ReadCsv();

    std::vector<std::complex<float>> vis = datafile.GetColumn<float, std::complex>("vis");
    //std::vector<std::complex<float>> vis = datafile.GetColumn("vis");

    int index = 0;
    for(auto & it: vis){
        std::cout << index << ": " << it.real() << ", " << it.imag() << std::endl;
        index++;
    }
    **/


#if __cpp_lib_to_chars >= 201611L
    std::from_chars(s.data(), s.data() + s.size(), value, std::chars_format::general);
    std::cout << value << std::endl;
    //std::cout << s.data() << std::endl;
    //std::cout << s.size() << std::endl;
#endif

    std::string s = "2.345";
    float value;

    std::from_chars(s.data(), s.data() + s.size(), value, std::chars_format::general);
    std::cout << value << std::endl;

}