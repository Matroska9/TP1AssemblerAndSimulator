#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "instruction.h"
#include "processor.h"
#include <QString>
#include <QVector>
#include <QDebug>

class Simulator : public QObject
{
    Q_OBJECT

public:
    explicit Simulator(QObject *parent = nullptr);
    ~Simulator();

    // Carga el vector de instrucciones desde un QByteArray
    void loadInstructionByteArray(const QByteArray &instructionArray);
    // Reinicia el procesador
    void restartProcessor();
    // Ejecuta la simulación del programa
    int runSimulation(bool debugMode = false);

signals:
    // Señal emitida cuando se pasa un valor al LCD
    void lcdSignalPassed(QString value);
    // Señal emitida cuando se pasa una instrucción
    void instSignalPassed(QString opcode, QString operando, QString accumulator, int programCounter);

private slots:
    // Slot para pasar el valor al LCD
    void passLcdValue(QString value);
    // Slot para pasar la instrucción
    void passInstValue(QString opcode, QString operando, QString accumulator, int programCounter);

private:
    // Puntero al procesador
    Processor *processor;
    // Vector de instrucciones
    QVector<Instruction> instructionVector;

    // Modo de depuración
    bool debugMode;

    // Analiza el QByteArray de instrucciones y lo convierte en instrucciones individuales
    void parseInstructionByteArray(const QByteArray &instructionArray);
    // Simula la ejecución completa del programa
    void simulateCompleteProgram();
    // Realiza un paso de la simulación
    void simulateStep();
};

#endif // SIMULATOR_H
