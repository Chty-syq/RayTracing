//
// Created by syq on 23-9-19.
//

#pragma once
#include <complex>

namespace equation {
    void SolveQuadraticReal(const vector<double>& coefficients, vector<double> &roots);
    void SolveCubicReal(const vector<double>& coefficients, vector<double> &roots);
    void SolveQuarticReal(const vector<double>& coefficients, vector<double> &roots);

    void SolveQuartic(const std::complex<double> coefficients[5], std::complex<double> roots[4]);
}

static std::complex<double> complex_sqrt(const std::complex<double> & z) {
    return pow(z, 1. / 2.);
}

static std::complex<double> complex_cbrt(const std::complex<double> & z) {
    return pow(z, 1. / 3.);
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

void equation::SolveQuartic(const std::complex<double> coefficients[5], std::complex<double> roots[4]) {
    const std::complex<double> a = coefficients[4];
    const std::complex<double> b = coefficients[3] / a;
    const std::complex<double> c = coefficients[2] / a;
    const std::complex<double> d = coefficients[1] / a;
    const std::complex<double> e = coefficients[0] / a;

    const std::complex<double> Q1 = c * c - 3. * b * d + 12. * e;
    const std::complex<double> Q2 = 2. * c * c * c - 9. * b * c * d + 27. * d * d + 27. * b * b * e - 72. * c * e;
    const std::complex<double> Q3 = 8. * b * c - 16. * d - 2. * b * b * b;
    const std::complex<double> Q4 = 3. * b * b - 8. * c;

    const std::complex<double> Q5 = complex_cbrt(Q2 / 2. + complex_sqrt(Q2 * Q2 / 4. - Q1 * Q1 * Q1));
    const std::complex<double> Q6 = (Q1 / Q5 + Q5) / 3.;
    const std::complex<double> Q7 = 2. * complex_sqrt(Q4 / 12. + Q6);

    roots[0] = (-b - Q7 - complex_sqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)) / 4.;
    roots[1] = (-b - Q7 + complex_sqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)) / 4.;
    roots[2] = (-b + Q7 - complex_sqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)) / 4.;
    roots[3] = (-b + Q7 + complex_sqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)) / 4.;
}
