#pragma once
#include <string>
#include <iostream>
#include <cmath>

struct Complex {
    double re;
    double im;

    double abs() const {
        return std::sqrt(re * re + im * im);
    }

    bool operator<(const Complex& other) const {
        return abs() < other.abs();
    }

    bool operator==(const Complex& other) const {
        return re == other.re && im == other.im;
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.re;
        if (c.im >= 0) os << "+";
        os << c.im << "i";
        return os;
    }
};

struct Student {
    int id;
    std::string name;
    int course;

    bool operator<(const Student& other) const {
        return id < other.id;
    }

    bool operator==(const Student& other) const {
        return id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << "[" << s.id << ": " << s.name << "]";
        return os;
    }
};

struct Teacher {
    int id;
    std::string name;
    std::string department;

    bool operator<(const Teacher& other) const {
        return id < other.id;
    }

    bool operator==(const Teacher& other) const {
        return id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Teacher& t) {
        os << "[" << t.id << ": " << t.name << "]";
        return os;
    }
};

