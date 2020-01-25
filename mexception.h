#ifndef MEXCEPTION_H
#define MEXCEPTION_H

#include <exception>
#include <string>

class Mexception: public std::exception
{
private:
    std::string _msg;

public:
    Mexception();

    Mexception(std::string msg);

    const char* what() const noexcept override final;
};

#endif /* MEXCEPTION_H */