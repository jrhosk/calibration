//
// Created by Josh Hoskins on 5/28/24.
//

#ifndef CALIBRATION_ANTENNASOLVE_H
#define CALIBRATION_ANTENNASOLVE_H

#include <complex>
#include <vector>


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "CalibrationSolverBase.h"

class AntennaSolve: public CalibrationSolverBase {
protected:

    boost::numeric::ublas::vector<std::complex<double>> pGainsArray;
    boost::numeric::ublas::vector<std::complex<double>> loss;

    boost::numeric::ublas::matrix<std::complex<double>> pModelArray;
    boost::numeric::ublas::matrix<std::complex<double>> pObservedArray;

public:
    explicit AntennaSolve(const boost::numeric::ublas::matrix<std::complex<double>> &);
    ~AntennaSolve();

    boost::numeric::ublas::vector<std::complex<double>> GetGains();

    void SetVis(boost::numeric::ublas::matrix<std::complex<double>> &);

    boost::numeric::ublas::vector<std::complex<double>> Loss() override;

    void Transform() override;
    void Step(double alpha) override;
    void Fit(int n_batch, double alpha) override;

};

#endif //CALIBRATION_ANTENNASOLVE_H
