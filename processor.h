#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "int24.h"
#include "instruction.h"
#include <QObject>

class Processor : public QObject
{
    Q_OBJECT

public:
    explicit Processor(QObject *parent = nullptr)
        : QObject(parent), accumulator(Int24(0)), programCounter(0) {}

    void restartProcessor();
    void executeInstruction(const Instruction& instr);

    // Métodos de acceso
    int getProgramCounter();
    int getAccumulator();

signals:
    // Señales emitidas
    void lcdSignalEmitted(QString value); // Señal emitida cuando hay una actualización en el LCD
    void instSignalEmitted(QString opcode, QString operando, QString  accumulator, int programCounter); // Señal emitida cuando se pasa una instrucción

private:
    Int24 accumulator;
    int programCounter;
};

#endif // PROCESSOR_H
