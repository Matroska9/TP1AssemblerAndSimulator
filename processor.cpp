#include "processor.h"

void Processor::restartProcessor()
{
    programCounter = 0;
    accumulator = Int24(0);
}

void Processor::executeInstruction(const Instruction& instr)
{
    int pc = ++programCounter;

    switch (instr.opcode) {
    case 0x01:  // LOAD: Carga un valor desde la memoria en un registro u otro lugar de almacenamiento.
        accumulator = instr.operand;
        break;

    case 0x02:  // PRINT: Suma dos valores y almacena el resultado en un registro.
        emit lcdSignalEmitted(QString::number(int(accumulator)));
        break;

    case 0x03:  // ADD: Suma dos valores y almacena el resultado en un registro.
        accumulator += instr.operand;
        break;

    case 0x04:  // SUB: Resta un valor de otro y almacena el resultado en un registro.
        accumulator -= instr.operand;
        break;

    case 0x05:  // MULT: Multiplica dos valores y almacena el resultado en un registro.
        accumulator *= instr.operand;
        break;

    case 0x06:  // DIV: Divide un valor entre otro y almacena el cociente en un registro.
        if (instr.operand != 0)
            accumulator /= instr.operand;
        else
            qDebug() << "Error: División por cero.";
        break;

    case 0x07:  // RSHF: Desplaza bits hacia la derecha.
        accumulator >>= 1;
        break;

    case 0x08:  // LSHF: Desplaza bits hacia la izquierda.
        accumulator <<= 1;
        break;

    case 0x09:  // AND: Realiza una operación AND entre dos valores y almacena el resultado en un registro.
        accumulator &= instr.operand;
        break;

    case 0x0A:  // OR: Realiza una operación OR entre dos valores y almacena el resultado en un registro.
        accumulator |= instr.operand;
        break;

    case 0x0B:  // JMP: Salta a una ubicación específica en el código.
        programCounter = int(instr.operand) - 1;
        break;

    default:
        qDebug() << "Error: Opcode no reconocido.";
        break;
    }

    emit instSignalEmitted(
        Instruction::opcodeToString(instr.opcode),
        QString::number(int(instr.operand)),
        QString::number(int(accumulator)),
        pc
        );
}

int Processor::getProgramCounter()
{
    return programCounter;
}

int Processor::getAccumulator()
{
    return programCounter;
}

