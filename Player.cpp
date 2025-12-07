//
// Created by zamfi on 07.12.2025.
//

#include "Player.h"

void Player::deepCopy(const Player& other) {
    coordonate = other.coordonate;
    viteza = other.viteza;
    if (other.nume) {
        nume = new std::string(*(other.nume));
    } else {
        nume = nullptr;
    }
}

void Player::elibereazaMemoria() {
    if (nume) {
        delete nume;
        nume = nullptr;
    }
}

Player::Player(double x, double y, double v, const std::string& n)
    : coordonate(x, y), viteza(v), nume(new std::string(n)) {}

Player::Player(const Player& other) : nume(nullptr) {
    deepCopy(other);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        elibereazaMemoria();
        deepCopy(other);
    }
    return *this;
}

Player::~Player() {
    elibereazaMemoria();
}

const Pozitie& Player::getPozitie() const {
    return coordonate;
}

void Player::miscare(char dir) {
    switch(dir) {
        case 'w': coordonate.move(0, viteza); break;
        case 's': coordonate.move(0, -viteza); break;
        case 'a': coordonate.move(-viteza, 0); break;
        case 'd': coordonate.move(viteza, 0); break;
    }
}

std::ostream& operator<<(std::ostream& out, const Player& p) {
    out << "Player '" << (p.nume ? *p.nume : "Anonim") << "' la " << p.coordonate;
    return out;
}