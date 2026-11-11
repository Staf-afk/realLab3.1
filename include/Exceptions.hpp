#pragma once
#include <stdexcept>
#include <string>

class TreeException : public std::runtime_error {
public:
    explicit TreeException(const std::string& msg)
        : std::runtime_error("[Tree] " + msg) {}  // не ну типа памяти много выделяем лишней но не критично и там тд 
};

class EmptyTreeException : public TreeException {
public:
    explicit EmptyTreeException(const std::string& ctx = "")
        : TreeException("tree is empty" + (ctx.empty() ? "" : ": " + ctx)) {}
};

class ElementNotFoundException : public TreeException {
public:
    explicit ElementNotFoundException(const std::string& ctx = "")
        : TreeException("element not found" + (ctx.empty() ? "" : ": " + ctx)) {}
};

class InvalidFormatException : public TreeException {
public:
    explicit InvalidFormatException(const std::string& ctx)
        : TreeException("invalid format: " + ctx) {}
};

class InvalidPathException : public TreeException {
public:
    explicit InvalidPathException(const std::string& ctx)
        : TreeException("invalid path: " + ctx) {}
};



