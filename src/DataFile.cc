//
// Created by jhoskins on 5/22/24.
//
#ifdef _WIN64
#include <windows_string.h>
#endif

#include <DataFile.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>


void DataFile::ReadCsv(const char *delimiter) {

    std::string line;
    bool fHeaderRead = false;

    while(std::getline(*this, line)) {

        char *c_line = new char[line.length() + 1];
        strcpy(c_line, line.c_str());
        if (!fHeaderRead){
            DataFile::ParseHeader(c_line, delimiter);

            fHeaderRead=true;
        } else {
            DataFile::ParseRow(c_line, delimiter);
        }

    }

}

 void DataFile::ParseRow(char *line, const char *delimiter=",") {
    char *data;
    char *pnt = line;
    std::string key;
    int columnNumber = 0;

    while((data = strsep(&pnt, delimiter)) != nullptr) {
        key = this->columns_[columnNumber];
        this->dataframe_[key].push_back(data);
        columnNumber++;
    }
}

void DataFile::ParseHeader(char *line, const char *delimiter=",") {
    char *header;
    char *pnt = line;

    while((header = strsep(&pnt, delimiter)) != nullptr) {
        this->dataframe_[std::string(header)] = std::vector<std::string>();
        this->columns_.emplace_back(header);
    }
}

std::pair <std::string, std::string> DataFile::ConvertComplex(const std::string& cnumber){
    std::size_t rPositivePosition = std::string::npos;
    std::size_t lPositivePosition = std::string::npos;

    std::size_t rNegativePosition = std::string::npos;
    std::size_t lNegativePosition = std::string::npos;

    std::string realString;
    std::string imaginaryString;

    rPositivePosition = cnumber.rfind("+");
    lPositivePosition = cnumber.find("+");

    rNegativePosition = cnumber.rfind("-");
    lNegativePosition = cnumber.find("-");

    if ( (rPositivePosition == lPositivePosition) && lPositivePosition != std::string::npos){
        if (rPositivePosition == 0){
            std::size_t length = cnumber.rfind("i") - rNegativePosition;

            realString = cnumber.substr(1, rNegativePosition-1);
            imaginaryString = cnumber.substr(rNegativePosition, length);
        } else {
            std::size_t length = cnumber.rfind("i") - rPositivePosition - 1;

            realString = cnumber.substr(0, rPositivePosition);
            imaginaryString = cnumber.substr(rPositivePosition + 1, length);
        }

    }

    if ( (rPositivePosition != lPositivePosition) && (lPositivePosition != std::string::npos) && (rPositivePosition != std::string::npos)){
        std::size_t length = cnumber.rfind("i") - rPositivePosition - 1;

        realString = cnumber.substr(1, rPositivePosition - 1);
        imaginaryString = cnumber.substr(rPositivePosition+1, length);
    }

    if ( (rNegativePosition != lNegativePosition) && (lNegativePosition != std::string::npos) && (rNegativePosition != std::string::npos)){
        std::size_t length = cnumber.rfind("i") - rNegativePosition - 1;

        realString = cnumber.substr(0, rNegativePosition);
        imaginaryString = cnumber.substr(rNegativePosition, length);
    }

    if ((rPositivePosition == lPositivePosition) && lPositivePosition == std::string::npos){
        std::size_t length = cnumber.rfind("i") - rNegativePosition;

        realString = cnumber.substr(0, rNegativePosition);
        imaginaryString = cnumber.substr(rNegativePosition, length);

    }

    return std::pair<std::string, std::string>{realString, imaginaryString};
}

void DataFile::PrintKeys() const{
    for (auto & it : this->columns_){
        std::cout << "key: " << it << std::endl;
    }
}

void DataFile::PrintDataFrame() const{
    for (auto & it : this->dataframe_){
        std::cout << "key: " << it.first << ": " << it.second.front() << std::endl;
    }
}