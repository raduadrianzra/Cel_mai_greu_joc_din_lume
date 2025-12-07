//
// Created by zamfi on 07.12.2025.
//

#ifndef OOP_POZITIE_H
#define OOP_POZITIE_H

#include <iostream>

class Pozitie {
    double x, y;

public:
    Pozitie(double x = 0, double y = 0);

    ~Pozitie();

    double getX() const;
    double getY() const;

    void set(double nx, double ny);
    void move(double dx, double dy);

    static double distanta(const Pozitie& a, const Pozitie& b);

    friend std::ostream& operator<<(std::ostream& out, const Pozitie& p);
};

#endif //OOP_POZITIE_H