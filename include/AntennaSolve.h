//
// Created by mystletainn on 5/28/24.
//

#include <complex>
#include <vector>

#include "CalibrationSolverBase.h"

#ifndef CALIBRATION_ANTENNASOLVE_H
#define CALIBRATION_ANTENNASOLVE_H

class AntennaSolve: CalibrationSolverBase {

public:
    void SetVis(std::vector<std::complex<float>>);
    void SetGains(std::vector<std::complex<float>>);

    void Transform() override;
    void Fit(int n_batch, float step) override;

};

#endif //CALIBRATION_ANTENNASOLVE_H
