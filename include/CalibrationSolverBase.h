//
// Created by jhoskins on 5/30/24.
//

#ifndef CALIBRATIONSOLVERBASE_H
#define CALIBRATIONSOLVERBASE_H

class CalibrationSolverBase {
public:
    //virtual ~CalibrationSolverBase() = 0;

    virtual void Step(double alpha) = 0;
    virtual void Transform() = 0;
    virtual boost::numeric::ublas::vector<std::complex<double>> Loss() = 0;
    virtual void Fit(int, double) = 0;
};

#endif //CALIBRATIONSOLVERBASE_H
