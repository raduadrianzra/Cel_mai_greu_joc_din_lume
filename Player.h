//
// Created by zamfi on 07.12.2025.
//

#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include "Pozitie.h"
#include <iostream>
#include <string>

class Player {
    Pozitie coordonate;
    double viteza;
    std::string* nume;

    void deepCopy(const Player& other);
    void elibereazaMemoria();

public:
    Player(double x, double y, double v, const std::string& n);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    const Pozitie& getPozitie() const;
    void miscare(char dir);

    friend std::ostream& operator<<(std::ostream& out, const Player& p);
};

#endif //OOP_PLAYER_H