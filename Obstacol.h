//
// Created by zamfi on 07.12.2025.
//

#ifndef OOP_OBSTACOL_H
#define OOP_OBSTACOL_H

#include "Pozitie.h"
#include <memory>

class Obstacol {
protected:
    Pozitie pozOBS;
    double raza;
    static int contorObstacole;

    void valideazaInvarianti(double x, double y, double r) const;

public:
    Obstacol(double x, double y, double r);
    virtual ~Obstacol();

    static int getNumarObstacole();
    bool verificaColiziune(const Pozitie& p) const;

    virtual void actioneaza(const Pozitie& pozitieJucator) = 0;
    virtual std::unique_ptr<Obstacol> clone() const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Obstacol& o);

protected:
    virtual std::ostream& afisare(std::ostream& out) const = 0;
};

// --- Clase Derivate ---

class ObstacolStatic : public Obstacol {
public:
    ObstacolStatic(double x, double y, double r);
    std::unique_ptr<Obstacol> clone() const override;
    void actioneaza(const Pozitie&) override;

protected:
    std::ostream& afisare(std::ostream& out) const override;
};

class ObstacolPatrula : public Obstacol {
    double viteza;
    double limitaSt, limitaDr;
    int directie;
public:
    ObstacolPatrula(double x, double y, double r, double v);
    std::unique_ptr<Obstacol> clone() const override;
    void actioneaza(const Pozitie&) override;

protected:
    std::ostream& afisare(std::ostream& out) const override;
};

class ObstacolUrmaritor : public Obstacol {
    double viteza;
public:
    ObstacolUrmaritor(double x, double y, double r, double v);
    std::unique_ptr<Obstacol> clone() const override;
    void actioneaza(const Pozitie& p) override;

protected:
    std::ostream& afisare(std::ostream& out) const override;
};

class ObstacolTeleport : public Obstacol {
    bool vizibil;
public:
    ObstacolTeleport(double x, double y, double r);
    std::unique_ptr<Obstacol> clone() const override;
    void actioneaza(const Pozitie& p) override;

protected:
    std::ostream& afisare(std::ostream& out) const override;
};

#endif //OOP_OBSTACOL_H