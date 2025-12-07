#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <algorithm>
#include <exception>
#include <fstream>

using namespace std;

class GameException : public std::exception {
protected:
    string mesaj;  //stocare mesaj de eroare
public:
    explicit GameException(const string& msg) : mesaj(msg) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

class EroareCoordonate : public GameException {
public:
    EroareCoordonate(const string& msg) : GameException("err coord: " + msg) {}
};

class EroareResursa : public GameException {
public:
    EroareResursa(const string& msg) : GameException("err resursa: " + msg) {}  //obstacole, raze, pozitii invalide etc.
};

class Pozitie {
    double x, y;

public:
    Pozitie(double x = 0, double y = 0) {
        this->x = x;
        this->y = y;
    }

    double getX() const { return x; }
    double getY() const { return y; }
    ~Pozitie() {}

    void set(double nx, double ny) { x = nx; y = ny; }
    void move(double dx, double dy) { x += dx; y += dy; }

    static double distanta(const Pozitie& a, const Pozitie& b) {  //static pentru ca ia 2 obiecte ca parametrii
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }

    friend ostream& operator<<(ostream& out, const Pozitie& p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }
};

class Obstacol {
protected:
    Pozitie pozOBS;
    double raza;
    static int contorObstacole;

    void valideazaInvarianti(double x, double y, double r) const {
        if (r <= 0) throw EroareResursa("aaza obstacolului >0");
        if (x < 0 || y < 0) throw EroareCoordonate("obstacol pus in afara hartii");
    }

public:
    Obstacol(double x, double y, double r) : pozOBS(x, y), raza(r) {
        valideazaInvarianti(x, y, r);
        contorObstacole++;
    }

    virtual ~Obstacol() {
        contorObstacole--;
    }

    static int getNumarObstacole() { return contorObstacole; }

    bool verificaColiziune(const Pozitie& p) const {
        return Pozitie::distanta(p, pozOBS) < raza;
    }

    virtual void actioneaza(const Pozitie& pozitieJucator) = 0;
    virtual std::unique_ptr<Obstacol> clone() const = 0;  //clone stocare pointeri la obstacole si returnare exacta

    //apeleaza <<, care apeleaza afisare
    friend ostream& operator<<(ostream& out, const Obstacol& o) {
        return o.afisare(out);
    }

protected:
    //doar derivatele au voie sa implementeze afisarea
    virtual ostream& afisare(ostream& out) const = 0;
};

int Obstacol::contorObstacole = 0;

class ObstacolStatic : public Obstacol {
public:
    ObstacolStatic(double x, double y, double r) : Obstacol(x, y, r) {}

    std::unique_ptr<Obstacol> clone() const override {
        return std::make_unique<ObstacolStatic>(*this);
    }

    void actioneaza(const Pozitie&) override {}

protected:
    ostream& afisare(ostream& out) const override {
        out << "static " << pozOBS << " r=" << raza;
        return out;
    }
};

class ObstacolPatrula : public Obstacol {
    double viteza;
    double limitaSt, limitaDr;
    int directie;
public:
    ObstacolPatrula(double x, double y, double r, double v)
        : Obstacol(x, y, r), viteza(v), limitaSt(x-2), limitaDr(x+2), directie(1) {}

    std::unique_ptr<Obstacol> clone() const override {
        return std::make_unique<ObstacolPatrula>(*this);
    }

    void actioneaza(const Pozitie&) override {
        pozOBS.move(viteza * directie, 0);
        if (pozOBS.getX() > limitaDr) directie = -1;
        if (pozOBS.getX() < limitaSt) directie = 1;
    }

protected:
    ostream& afisare(ostream& out) const override {
        out << "[Patrula] " << pozOBS << " v=" << viteza;
        return out;
    }
};

class ObstacolUrmaritor : public Obstacol {
    double viteza;
public:
    ObstacolUrmaritor(double x, double y, double r, double v)
        : Obstacol(x, y, r), viteza(v) {}

    std::unique_ptr<Obstacol> clone() const override {
        return std::make_unique<ObstacolUrmaritor>(*this);
    }

    void actioneaza(const Pozitie& p) override {
        double dist = Pozitie::distanta(p, pozOBS);
        if (dist > 0.1) {
            double dx = p.getX() - pozOBS.getX();
            double dy = p.getY() - pozOBS.getY();
            pozOBS.move((dx/dist)*viteza, (dy/dist)*viteza);
        }
    }

protected:
    ostream& afisare(ostream& out) const override {
        out << "urmaritor " << pozOBS << " (te vede)";
        return out;
    }
};

class Player {
    Pozitie coordonate;
    double viteza;
    string* nume;

    void deepCopy(const Player& other) {
        coordonate = other.coordonate;
        viteza = other.viteza;
        if (other.nume) {
            nume = new string(*(other.nume));
        } else {
            nume = nullptr;
        }
    }

    void elibereazaMemoria() {
        if (nume) {
            delete nume;
            nume = nullptr;
        }
    }

public:
    Player(double x, double y, double v, const string& n)
        : coordonate(x, y), viteza(v), nume(new string(n)) {}

    Player(const Player& other) : nume(nullptr) {
        deepCopy(other);
    }

    Player& operator=(const Player& other) {
        if (this != &other) {
            elibereazaMemoria();
            deepCopy(other);
        }
        return *this;
    }

    ~Player() {
        elibereazaMemoria();
    }

    const Pozitie& getPozitie() const { return coordonate; }

    void miscare(char dir) {
        switch(dir) {
            case 'w': coordonate.move(0, viteza); break;
            case 's': coordonate.move(0, -viteza); break;
            case 'a': coordonate.move(-viteza, 0); break;
            case 'd': coordonate.move(viteza, 0); break;
        }
    }

    friend ostream& operator<<(ostream& out, const Player& p) {
        out << "Player '" << (p.nume ? *p.nume : "Anonim") << "' la " << p.coordonate;
        return out;
    }
};

class Nivel {
    Player jucator;
    vector<unique_ptr<Obstacol>> obstacole;
    Pozitie final;
    bool activ;

public:
    Nivel(Player p, Pozitie f) : jucator(p), final(f), activ(true) {}

    Nivel(const Nivel& other) : jucator(other.jucator), final(other.final), activ(other.activ) {
        for (const auto& obs : other.obstacole) {
            obstacole.push_back(obs->clone());
        }
    }

    Nivel& operator=(Nivel other) {
        swap(*this, other);
        return *this;
    }

    friend void swap(Nivel& a, Nivel& b) noexcept {
        using std::swap;
        swap(a.jucator, b.jucator);
        swap(a.obstacole, b.obstacole);
        swap(a.final, b.final);
        swap(a.activ, b.activ);
    }

    void adaugaObstacol(unique_ptr<Obstacol> o) {
        obstacole.push_back(std::move(o));
    }

    bool update(char input) {
        if (!activ) return false;

        jucator.miscare(input);

        for (auto& o : obstacole) {
            o->actioneaza(jucator.getPozitie());

            if (o->verificaColiziune(jucator.getPozitie())) {
                cout << "!!! coliziune cu: " << *o << "\n";
                activ = false;
                return false;
            }

            if (dynamic_cast<ObstacolUrmaritor*>(o.get())) {
            }
        }

        if (Pozitie::distanta(jucator.getPozitie(), final) < 1.0) {
            cout << "yay!\n";
            activ = false;
            return false;
        }
        return true;
    }

    friend ostream& operator<<(ostream& out, const Nivel& n) {
        out << "\nStare Joc:\n" << n.jucator << "\nObstacole active: " << n.obstacole.size() << "\n";
        for(const auto& o : n.obstacole) out << " -> " << *o << "\n";
        return out;
    }
};

class ObstacolTeleport : public Obstacol {
    bool vizibil;
public:
    ObstacolTeleport(double x, double y, double r) : Obstacol(x, y, r), vizibil(true) {}

    std::unique_ptr<Obstacol> clone() const override {
        return std::make_unique<ObstacolTeleport>(*this);
    }

    void actioneaza(const Pozitie& /*p*/) override {
        vizibil = !vizibil;
        if (!vizibil) pozOBS.set(-100, -100);
        else pozOBS.set(5, 5);
    }

protected:
    ostream& afisare(ostream& out) const override {
        out << "teleportare " << (vizibil ? "vizibil" : "nu vizibil");
        return out;
    }
};

int main() {
    try {
        cout << "initializare\n";

        Player p(0, 0, 1.0, "player");
        Nivel nivel(p, Pozitie(10, 10));

        nivel.adaugaObstacol(make_unique<ObstacolStatic>(3, 3, 0.5));
        nivel.adaugaObstacol(make_unique<ObstacolPatrula>(5, 5, 0.5, 0.3));
        nivel.adaugaObstacol(make_unique<ObstacolUrmaritor>(8, 8, 0.5, 0.2));
        nivel.adaugaObstacol(make_unique<ObstacolTeleport>(5, 2, 0.5));

        //nivel.adaugaObstacol(make_unique<ObstacolStatic>(-1, 0, 0.5));

        ifstream fin("tastatura.txt");
        char input;
        bool useFile = fin.is_open();

        while (true) {
            cout << nivel;

            if (useFile) {
                fin >> input;
                if (fin.eof()) break;
                cout << "input fis: " << input << endl;
            } else {
                cout << "Comanda (w/a/s/d): ";
                cin >> input;
            }

            if (!nivel.update(input)) break;
        }

    } catch (const GameException& e) {
        cerr << "\nEXCEPTIE JOC: " << e.what() << endl;
    } catch (const std::exception& e) {
        cerr << "\nEXCEPTIE SISTEM: " << e.what() << endl;
    }

    return 0;
}