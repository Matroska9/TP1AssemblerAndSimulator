#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <int24.h>
#include <QMap>


struct InstructionPropieties {
    char opcode;
    bool usesOperand;
};

static const QMap<QString, InstructionPropieties> InstructionSet = {
    {"LOAD", {0x01, true}},     // Carga un valor desde la memoria en un registro u otro lugar de almacenamiento.
    {"PRINT", {0x02, false}},   // Imprime el valor del acumulador en un LCD.
    {"ADD", {0x03, true}},      // Suma dos valores y almacena el resultado en un registro.
    {"SUB", {0x04, true}},      // Resta un valor de otro y almacena el resultado en un registro.
    {"MULT", {0x05, true}},     // Multiplica dos valores y almacena el resultado en un registro.
    {"DIV", {0x06, true}},      // Divide un valor entre otro y almacena el cociente en un registro.
    {"RSHF", {0x07, false}},    // Desplaza bits hacia la derecha.
    {"LSHF", {0x08, false}},    // Desplaza bits hacia la izquierda.
    {"AND", {0x09, true}},      // Realiza una operación AND entre dos valores y almacena el resultado en un registro.
    {"OR", {0x0A, true}},       // Realiza una operación OR entre dos valores y almacena el resultado en un registro.
    {"JMP", {0x0B, true}}       // Salta a una ubicación específica en el código.
};


struct Instruction {
    char opcode;
    Int24 operand;

    static QString opcodeToString(char opcodeToFind)
    {
        for (auto it = InstructionSet.constBegin(); it != InstructionSet.constEnd(); ++it) {
            if (it.value().opcode == opcodeToFind) {
                return it.key();
            }
        }

        // Si no se encuentra ninguna coincidencia, devolver una cadena vacía
        return "";
    }
};

#endif // INSTRUCTION_H
