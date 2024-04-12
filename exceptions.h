#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Excepción base para todas las excepciones personalizadas
class CustomException : public std::runtime_error {
public:
    CustomException(const std::string& message) : std::runtime_error(message) {}
};

// Excepción para errores relacionados con el archivo
class FileException : public CustomException {
public:
    FileException(const std::string& message) : CustomException(message) {}
};

// Excepción para errores relacionados con la lectura de instrucciones
class InvalidInstructionException : public CustomException {
public:
    InvalidInstructionException(const std::string& message) : CustomException(message) {}
};

// Excepción para errores relacionados con la lectura de instrucciones
class InvalidInstructionExceptionWithLine : public CustomException {
public:
    InvalidInstructionExceptionWithLine(const std::string& message, int lineNumber)
        : CustomException(message + "\nError en línea: " + std::to_string(lineNumber)), lineNumber(lineNumber) {}

    int getLineNumber() const { return lineNumber; }

private:
    int lineNumber;
};

#endif // EXCEPTIONS_H
