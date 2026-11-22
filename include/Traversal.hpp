#pragma once
#include "Exceptions.hpp"
#include <string>
#include <array>

enum class Traversal {
    KLP,  
    KPL,
    LPK, 
    LKP,
    PLK,
    PKL  
};

inline Traversal traversalFromString(const std::string& s) {
    if (s == "KLP") return Traversal::KLP;
    if (s == "KPL") return Traversal::KPL;
    if (s == "LPK") return Traversal::LPK;
    if (s == "LKP") return Traversal::LKP;
    if (s == "PLK") return Traversal::PLK;
    if (s == "PKL") return Traversal::PKL;
    throw InvalidFormatException("unknown traversal '" + s + "'");
}

inline std::string traversalToString(Traversal t) {
    switch (t) {
        case Traversal::KLP: return "KLP";
        case Traversal::KPL: return "KPL";
        case Traversal::LPK: return "LPK";
        case Traversal::LKP: return "LKP";
        case Traversal::PLK: return "PLK";
        case Traversal::PKL: return "PKL";
    }
    return "?";
}

inline std::array<int,3> traversalOrder(Traversal t) {
    switch (t) {
        case Traversal::KLP: return {0,1,2};
        case Traversal::KPL: return {0,2,1};
        case Traversal::LPK: return {1,2,0};
        case Traversal::LKP: return {1,0,2};
        case Traversal::PLK: return {2,1,0};
        case Traversal::PKL: return {2,0,1};
    }
    return {0,1,2};
}


