//
// Created by zamfi on 07.12.2025.
//

#ifndef OOP_NIVEL_H
#define OOP_NIVEL_H

#include "Player.h"
#include "Obstacol.h"
#include <vector>
#include <memory>

class Nivel {
    Player jucator;
    std::vector<std::unique_ptr<Obstacol>> obstacole;
    Pozitie final;
    bool activ;

public:
    Nivel(Player p, Pozitie f);
    Nivel(const Nivel& other);
    Nivel& operator=(Nivel other);

    // Functie friend pentru swap
    friend void swap(Nivel& a, Nivel& b) noexcept;

    void adaugaObstacol(std::unique_ptr<Obstacol> o);
    bool update(char input);

    friend std::ostream& operator<<(std::ostream& out, const Nivel& n);
};

#endif //OOP_NIVEL_H