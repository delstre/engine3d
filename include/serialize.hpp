#pragma once

#include <fstream>

class OArchive {
    public:
        OArchive(std::ofstream& ofs) : ofs_(ofs) {}

        template<typename T>
        OArchive& operator<<(const T& a);

        std::ofstream& get();

    private:
        std::ofstream& ofs_;
};

class IArchive {
    public:
        IArchive(std::ifstream& ifs) : ifs_(ifs) {}

        template<typename T>
        IArchive& operator>>(T& a);

        std::ifstream& get();

    private:
        std::ifstream& ifs_;
};

