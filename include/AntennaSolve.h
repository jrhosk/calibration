//
// Created by mystletainn on 5/28/24.
//

#ifndef CALIBRATION_ANTENNASOLVE_H
#define CALIBRATION_ANTENNASOLVE_H

#include <complex>
#include <vector>

#include "CalibrationSolverBase.h"

class AntennaSolve: public CalibrationSolverBase {
protected:
    std::vector<std::complex<float>> pModelArray;
    std::vector<std::complex<float>> pGainsArray;
    std::vector<std::complex<float>> pObservedArray;

public:
    ~AntennaSolve();

    std::vector<std::complex<float>> GetGains();
    void SetVis(std::vector<std::complex<float>>);
    void SetGains(std::vector<std::complex<float>>);

    std::complex<float> Loss(const std::complex<float> &, const std::complex<float> &) override;

    void Transform() override;
    void Fit(int n_batch, float step) override;

};

#endif //CALIBRATION_ANTENNASOLVE_H
