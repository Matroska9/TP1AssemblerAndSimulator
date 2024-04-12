#include "simulator.h"
#include "exceptions.h"
#include <QMessageBox>

Simulator::Simulator(QObject *parent) :
    QObject(parent),
    processor(new Processor()),
    debugMode(false)
{
    // Conección de señales
    connect(processor, &Processor::lcdSignalEmitted, this, &Simulator::passLcdValue);
    connect(processor, &Processor::instSignalEmitted, this, &Simulator::passInstValue);
}

Simulator::~Simulator() {
    delete processor;
}

// Slots para pasar los valores del LCD e instrucción emitidos por el procesador
void Simulator::passLcdValue(QString  value)
{
    emit lcdSignalPassed(value);
}

void Simulator::passInstValue(QString opcode, QString operando, QString  accumulator, int programCounter)
{
    emit instSignalPassed(opcode, operando, accumulator, programCounter);
}


// Carga el arreglo de instrucciones en el simulador
void Simulator::loadInstructionByteArray(const QByteArray& instructionArray)
{
    instructionVector.clear();
    parseInstructionByteArray(instructionArray);
}

// Reinicia el procesador
void Simulator::restartProcessor()
{
    processor->restartProcessor();
}

// Ejecuta la simulación del programa, con opción de modo de depuración
int Simulator::runSimulation(bool debugMode)
{
    if(debugMode)
    {
        simulateStep();
    }
    else
    {
        simulateCompleteProgram();
    }
    return processor->getProgramCounter();
}

void Simulator::simulateStep()
{
    int pc = processor->getProgramCounter();
    if(pc < instructionVector.length())
        processor->executeInstruction(instructionVector.at(pc));
}

void Simulator::simulateCompleteProgram()
{
    processor->restartProcessor();

    int maxIterations = 100;
    while(processor->getProgramCounter() < instructionVector.length())
    {
        int pc = processor->getProgramCounter();
        processor->executeInstruction(instructionVector.at(pc));

        if(maxIterations-- < 0)
        {
            throw FileException("El programa tiene un bucle infinito.");
        }
    }
}

// Analiza el arreglo de bytes de instrucciones y crea las instrucciones correspondientes
void Simulator::parseInstructionByteArray(const QByteArray& instructionArray)
{
    instructionVector.clear();

    for(int i = 0; i + 4 <= instructionArray.length(); i += 4)
    {
        Instruction instr;
        instr.opcode = instructionArray[i];
        instr.operand = Int24(const_cast<char*>(instructionArray.data() + i + 1));
        instructionVector.append(instr);
    }
}
