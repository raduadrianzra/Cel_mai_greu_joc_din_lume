#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Pozitie {
    double x,y;  //coordonate
public:
    Pozitie(double x = 0, double y = 0) {
    this-> x = x;
    this-> y = y;
    }
    ~Pozitie() {}

    double getX() const { return x; }
    double getY() const { return y; }

    void move(double dX, double dY) {x+= dX; y+= dY;}  //adauga dX si dY la coord

    friend ostream& operator<<(ostream& out, const Pozitie& p) {  //supraincarcare
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }
};

class Obstacol {
    Pozitie pozOBS;
    double size;
public:
    Obstacol(double x = 0, double y = 0, double s = 1.0) {
        this->pozOBS = Pozitie(x,y);
        this->size = s;
    }
    ~Obstacol() {}

    bool collides(const Pozitie& p) const {  //verifica daca p este in interiorul obstacolului
        return (abs(p.getX()-pozOBS.getX()) < size) && (abs(p.getY()-pozOBS.getY()) < size);
    }  //compara dist pe x si y fata de poz obst si compara cu marimea, daca sunt mai mici rezutla coliziune

    friend ostream& operator<<(ostream& out, const Obstacol& o) {  //supraincarcare
        out << "obstacol" << o.pozOBS << " " << o.size;
        return out;
    }

};

class Player {
    Pozitie pozPLR;
    double viteza;
public:
    Player(double x = 0, double y = 0, double v = 1.0) {
        this->pozPLR = Pozitie(x,y);  //seteza viteza si poz initiala
        this->viteza = v;
    }

    Player(const Player& noua) {//copiaza poz si viteaza
        this->pozPLR = noua.pozPLR;
        this->viteza = noua.viteza;
    }

    Player& operator=(const Player& noua) {
        if (this != &noua) {
            pozPLR = noua.pozPLR;
            viteza = noua.viteza;
        }
        return *this;
    }

    void miscare(char directie) {
        switch (directie) {
            case 'w': pozPLR.move(0, viteza); break;
            case 's': pozPLR.move(0, -viteza); break;
            case 'a': pozPLR.move(-viteza, 0); break;
            case 'd': pozPLR.move(viteza, 0); break;
        }
    }

    const Pozitie& getPozPLR() const { return pozPLR; }  //returneaza poz

    friend ostream& operator<<(ostream& out, const Player& p) {
        out << "poz" << p.pozPLR << "\n";
        return out;
    }

    ~Player() {}
};

class Nivel {
    Player jucator;
    vector<Obstacol> obstacole;
    Pozitie final;
public:
    Nivel(Player p, Pozitie f) {
        this-> jucator = p;
        this-> final = f;
    }

    void addObstacol(const Obstacol& o) {
        obstacole.push_back(o);
    }

    bool checkCollision() const {
        for (const auto& o :obstacole)
            if (o.collides(jucator.getPozPLR())) return true;
        return false;
    }

    bool ajunsfinal() const {
        return (abs(jucator.getPozPLR().getX() - final.getX()) < 1.0 && abs(jucator.getPozPLR().getY() - final.getY()) < 1.0);
    }

    void update(char input) {
        jucator.miscare(input);
        if (checkCollision())
            cout << "obstacol\n";
        if (ajunsfinal())
            cout << "yay\n";
    }

    friend ostream& operator<<(ostream& out, const Nivel& n) {
        out << "nivel\n"<< n.jucator << "final"<< n.final << "\nobstacole\n";
        for (const auto& o : n.obstacole)
            out << "-" << o << "\n";
        return out;
    }
};

int main() {
    ifstream fin ("tastatura.txt");

    Player p(0,0,1.0);
    Nivel n(p, Pozitie(5,5));

    n.addObstacol(Obstacol(2, 0, 0.8));
    n.addObstacol(Obstacol(3, 3, 0.7));

    cout << n << endl;  //mut aici yay daca vr

    char input;
    while (fin >> input) {
        n.update(input);
        cout << n << endl;
        if (n.ajunsfinal()) break;
    }

    return 0;
}
