//
// Created by zamfi on 07.12.2025.
//
#include "Pozitie.h"
#include <cmath>

Pozitie::Pozitie(double x, double y) {
    this->x = x;
    this->y = y;
}

Pozitie::~Pozitie() {}

double Pozitie::getX() const { return x; }
double Pozitie::getY() const { return y; }

void Pozitie::set(double nx, double ny) {
    x = nx; y = ny;
}

void Pozitie::move(double dx, double dy) {
    x += dx; y += dy;
}

double Pozitie::distanta(const Pozitie& a, const Pozitie& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

std::ostream& operator<<(std::ostream& out, const Pozitie& p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}