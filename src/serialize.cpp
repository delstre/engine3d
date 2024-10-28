#include <serialize.hpp>
#include <reflection.hpp>
#include <component.hpp>
#include <object.hpp>

#include <glm/glm.hpp>

template<typename T>
OArchive& OArchive::operator<<(const T& a) {
    ofs_.write(reinterpret_cast<const char*>(&a), sizeof(T));
    return *this;
}

std::ofstream& OArchive::get() {
    return ofs_;
}

template<typename T>
IArchive& IArchive::operator>>(T& a) {
    ifs_.read(reinterpret_cast<char*>(&a), sizeof(T));
    return *this;
}

std::ifstream& IArchive::get() {
    return ifs_;
}


template OArchive& OArchive::operator<< <unsigned long>(const unsigned long&);
template IArchive& IArchive::operator>> <unsigned long>(unsigned long&);

template OArchive& OArchive::operator<< <float>(const float&);
template IArchive& IArchive::operator>> <float>(float&);

template OArchive& OArchive::operator<< <char const*>(char const* const&);
template IArchive& IArchive::operator>> <char const*>(char const*&);

template OArchive& OArchive::operator<< <void*>(void* const&);
template IArchive& IArchive::operator>> <void*>(void*&);

template OArchive& OArchive::operator<< <int>(const int&);
template IArchive& IArchive::operator>> <int>(int&);

template OArchive& OArchive::operator<< <glm::vec<3, float>>(const glm::vec<3, float>&);
template IArchive& IArchive::operator>> <glm::vec<3, float>>(glm::vec<3, float>&);
