#include "mexception.h"

Mexception::Mexception(): _msg(std::string("Unexpected error occurred"))
{
}

Mexception::Mexception(std::string msg): _msg(msg)
{
}

const char* Mexception::what() const noexcept
{
    return _msg.c_str();
}