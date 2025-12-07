//
// Created by zamfi on 07.12.2025.
//

#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string mesaj;
public:
    explicit GameException(const std::string& msg) : mesaj(msg) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

class EroareCoordonate : public GameException {
public:
    EroareCoordonate(const std::string& msg) : GameException("err coord: " + msg) {}
};

class EroareResursa : public GameException {
public:
    EroareResursa(const std::string& msg) : GameException("err resursa: " + msg) {}
};

#endif //OOP_EXCEPTII_H