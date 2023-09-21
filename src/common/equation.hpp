//
// Created by syq on 23-9-19.
//

#pragma once
#include <complex>

namespace equation {
    void SolveQuadraticReal(const vector<double>& coefficients, vector<double> &roots);
    void SolveCubicReal(const vector<double>& coefficients, vector<double> &roots);
    void SolveQuarticReal(const vector<double>& coefficients, vector<double> &roots);
}

void equation::SolveQuadraticReal(const vector<double>& coefficients, vector<double> &roots) { //ax^2 + bx + c = 0
    const double a = coefficients[0];
    const double b = coefficients[1];
    const double c = coefficients[2];
    const double discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0)  return;  //无解
    roots.push_back((-b - sqrt(discriminant)) / (2. * a));
    roots.push_back((-b + sqrt(discriminant)) / (2. * a));
}

void equation::SolveCubicReal(const vector<double>& coefficients, vector<double> &roots) { //ax^3 + bx^2 + cx + d = 0
    const double a = coefficients[0];
    const double b = coefficients[1];
    const double c = coefficients[2];
    const double d = coefficients[3];
    const double p = (c / a - b * b / (3. * a * a)) / 3.;
    const double q = (d / a + 2. * b * b * b / (27. * a * a * a) - b * c / (3. * a * a)) / 2.;
    const double diff = -b / (3. * a);
    const double discriminant = q * q + p * p * p;
    if (discriminant > 0) { //一个根
        const double y = cbrt(sqrt(discriminant) - q) + cbrt(-sqrt(discriminant) - q);
        roots.push_back(y);
    }
    else if (discriminant == 0) { //两个根
        const double y1 = -2. * cbrt(q);
        const double y2 = cbrt(q);
        roots.insert(roots.end(), { y1, y2 });
    }
    else { //三个根
        const double alpha = acos(-q * sqrt(-p) / (p * p)) / 3.;
        const double y1 = 2. * sqrt(-p) * cos(alpha);
        const double y2 = 2. * sqrt(-p) * cos(alpha + 2. * PI / 3.);
        const double y3 = 2. * sqrt(-p) * cos(alpha + 4. * PI / 3.);
        roots.insert(roots.end(), { y1, y2, y3 });
    }
    for(auto &root: roots)  root += diff;
}

void equation::SolveQuarticReal(const vector<double>& coefficients, vector<double> &roots) { //ax^4 + bx^3 + cx^2 + dx + e = 0
    const double a = coefficients[0];
    const double b = coefficients[1] / a;
    const double c = coefficients[2] / a;
    const double d = coefficients[3] / a;
    const double e = coefficients[4] / a;

    vector<double> roots_y;
    SolveCubicReal({ 1., -c, b * d - 4. * e, 4. * c * e - b * b * e - d * d }, roots_y);
    const double y = roots_y[0];

    const double m = sqrt(b * b / 4. - c + y);
    const double n = (b * y / 2. - d) / (2. * m);

    SolveQuadraticReal({ 1., b / 2. + m, y / 2. + n }, roots);
    SolveQuadraticReal({ 1., b / 2. - m, y / 2. - n }, roots);
}
