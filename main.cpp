#include <iostream>
#include <fstream>
#include <memory>
#include "Pozitie.h"
#include "Player.h"
#include "Obstacol.h"
#include "Nivel.h"
#include "Exceptii.h"

using namespace std;

int main() {
    try {
        cout << "initializare\n";

        Player p(0, 0, 1.0, "player");
        Nivel nivel(p, Pozitie(10, 10));

        nivel.adaugaObstacol(make_unique<ObstacolStatic>(3, 3, 0.5));
        nivel.adaugaObstacol(make_unique<ObstacolPatrula>(5, 5, 0.5, 0.3));
        nivel.adaugaObstacol(make_unique<ObstacolUrmaritor>(8, 8, 0.5, 0.2));
        nivel.adaugaObstacol(make_unique<ObstacolTeleport>(5, 2, 0.5));

        // nivel.adaugaObstacol(make_unique<ObstacolStatic>(-1, 0, 0.5));

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