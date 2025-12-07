//
// Created by zamfi on 07.12.2025.
//

#include "Nivel.h"

Nivel::Nivel(Player p, Pozitie f) : jucator(p), final(f), activ(true) {}

Nivel::Nivel(const Nivel& other) : jucator(other.jucator), final(other.final), activ(other.activ) {
    for (const auto& obs : other.obstacole) {
        obstacole.push_back(obs->clone());
    }
}

Nivel& Nivel::operator=(Nivel other) {
    swap(*this, other);
    return *this;
}

void swap(Nivel& a, Nivel& b) noexcept {
    using std::swap;
    swap(a.jucator, b.jucator);
    swap(a.obstacole, b.obstacole);
    swap(a.final, b.final);
    swap(a.activ, b.activ);
}

void Nivel::adaugaObstacol(std::unique_ptr<Obstacol> o) {
    obstacole.push_back(std::move(o));
}

bool Nivel::update(char input) {
    if (!activ) return false;

    jucator.miscare(input);

    for (auto& o : obstacole) {
        o->actioneaza(jucator.getPozitie());

        if (o->verificaColiziune(jucator.getPozitie())) {
            std::cout << "!!! coliziune cu: " << *o << "\n";
            activ = false;
            return false;
        }

        if (dynamic_cast<ObstacolUrmaritor*>(o.get())) {
            // Logica specifica daca e necesar
        }
    }

    if (Pozitie::distanta(jucator.getPozitie(), final) < 1.0) {
        std::cout << "yay!\n";
        activ = false;
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Nivel& n) {
    out << "\nStare Joc:\n" << n.jucator << "\nObstacole active: " << n.obstacole.size() << "\n";
    for(const auto& o : n.obstacole) out << " -> " << *o << "\n";
    return out;
}