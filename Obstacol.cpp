//
// Created by zamfi on 07.12.2025.
//

#include "Obstacol.h"
#include "Exceptii.h"

int Obstacol::contorObstacole = 0;

void Obstacol::valideazaInvarianti(double x, double y, double r) const {
    if (r <= 0) throw EroareResursa("raza obstacolului >0");
    if (x < 0 || y < 0) throw EroareCoordonate("obstacol pus in afara hartii");
}

Obstacol::Obstacol(double x, double y, double r) : pozOBS(x, y), raza(r) {
    valideazaInvarianti(x, y, r);
    contorObstacole++;
}

Obstacol::~Obstacol() {
    contorObstacole--;
}

int Obstacol::getNumarObstacole() { return contorObstacole; }

bool Obstacol::verificaColiziune(const Pozitie& p) const {
    return Pozitie::distanta(p, pozOBS) < raza;
}

std::ostream& operator<<(std::ostream& out, const Obstacol& o) {
    return o.afisare(out);
}

ObstacolStatic::ObstacolStatic(double x, double y, double r) : Obstacol(x, y, r) {}

std::unique_ptr<Obstacol> ObstacolStatic::clone() const {
    return std::make_unique<ObstacolStatic>(*this);
}

void ObstacolStatic::actioneaza(const Pozitie&) {}

std::ostream& ObstacolStatic::afisare(std::ostream& out) const {
    out << "static " << pozOBS << " r=" << raza;
    return out;
}

ObstacolPatrula::ObstacolPatrula(double x, double y, double r, double v)
    : Obstacol(x, y, r), viteza(v), limitaSt(x - 2), limitaDr(x + 2), directie(1) {}

std::unique_ptr<Obstacol> ObstacolPatrula::clone() const {
    return std::make_unique<ObstacolPatrula>(*this);
}

void ObstacolPatrula::actioneaza(const Pozitie&) {
    pozOBS.move(viteza * directie, 0);
    if (pozOBS.getX() > limitaDr) directie = -1;
    if (pozOBS.getX() < limitaSt) directie = 1;
}

std::ostream& ObstacolPatrula::afisare(std::ostream& out) const {
    out << "[Patrula] " << pozOBS << " v=" << viteza;
    return out;
}

ObstacolUrmaritor::ObstacolUrmaritor(double x, double y, double r, double v)
    : Obstacol(x, y, r), viteza(v) {}

std::unique_ptr<Obstacol> ObstacolUrmaritor::clone() const {
    return std::make_unique<ObstacolUrmaritor>(*this);
}

void ObstacolUrmaritor::actioneaza(const Pozitie& p) {
    double dist = Pozitie::distanta(p, pozOBS);
    if (dist > 0.1) {
        double dx = p.getX() - pozOBS.getX();
        double dy = p.getY() - pozOBS.getY();
        pozOBS.move((dx / dist) * viteza, (dy / dist) * viteza);
    }
}

std::ostream& ObstacolUrmaritor::afisare(std::ostream& out) const {
    out << "urmaritor " << pozOBS << " (te vede)";
    return out;
}

ObstacolTeleport::ObstacolTeleport(double x, double y, double r)
    : Obstacol(x, y, r), vizibil(true) {}

std::unique_ptr<Obstacol> ObstacolTeleport::clone() const {
    return std::make_unique<ObstacolTeleport>(*this);
}

void ObstacolTeleport::actioneaza(const Pozitie&) {
    vizibil = !vizibil;
    if (!vizibil) pozOBS.set(-100, -100);
    else pozOBS.set(5, 5);
}

std::ostream& ObstacolTeleport::afisare(std::ostream& out) const {
    out << "teleportare " << (vizibil ? "vizibil" : "nu vizibil");
    return out;
}