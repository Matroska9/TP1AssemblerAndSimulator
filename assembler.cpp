#include "assembler.h"

Assembler::Assembler() {}


/* Genera .bin a partir del QString que corresponde al programa en assembler */
/* lo guarda en el mismo directorio y con el mismo nombre del .txt importado */
QString Assembler::generateBin(QString inTxt, QString inTxtFileName)
{
    QVector<Instruction*> instVector;
    Instruction *inst;
    QString line;

    try
    {
        if (inTxtFileName.isEmpty())
        {
            throw FileException("No hay ningún archivo de instrucciones abierto.");
        }

        QString outBinFileName = createBinFileName(inTxtFileName);

        QTextStream inTxtStream(&inTxt);
        while (!inTxtStream.atEnd())
        {
            line = inTxtStream.readLine();
            inst = new Instruction(readInstructionFromLine(line));
            instVector.append(inst);
        }

        saveInstructionVectorToBinFile(instVector, outBinFileName);
        return outBinFileName;
    }
    catch (const InvalidInstructionException& e) {
        int pos = instVector.length()+1;
        for (auto inst : instVector) {
            delete inst;
        }
        instVector.clear();
        throw InvalidInstructionExceptionWithLine(e.what(), pos);
    }
    catch (const FileException& e) {
        for (auto inst : instVector) {
            delete inst;
        }
        instVector.clear();
        throw;
    }
}

/* Cambia la terminación del QString de .txt a .bin */
QString Assembler::createBinFileName(const QString& inTxtFileName)
{
    return (inTxtFileName.left(inTxtFileName.lastIndexOf('.')) + ".bin");
}

/* Lee una línea del programa, separa el opcode y el operando, crea y devuelve un objeto instruction */
/* Genera una excepción si el comando no está en el set de instrucciones */
/* Genera una excepción si el operador no es un número de 3 bytes */
Instruction Assembler::readInstructionFromLine(const QString& inTxt)
{
    Instruction inst;

    // LECTURA OPCODE
    // extrae la primer palabra, la traduce al char correspondiente
    // si no existe la instrucción hace un trhow
    QString opcodeString = inTxt.split(" ").at(0);
    char opcodeChar = translateOpcodeToChar(opcodeString);
    if(!opcodeChar)
    {
        throw InvalidInstructionException("Instrucción inválida: " + opcodeString.toStdString());
    }

    // LECTURA OPERANDO
    // extrae el resto de la línea, eliminando espacios
    QString operandString = (opcodeString.length() < inTxt.length()) ?
                                inTxt.sliced(opcodeString.length()+1).remove(' ') : "";

    // convierte la substring a int
    bool isNumber = false;
    int operandInt = operandString.toInt(&isNumber);

    // comprueba si el opcode requiere un operando
    bool usesOperand = this->usesOperand(opcodeString);
    // comprueba si el opcode tiene tiene formato de signed int 24bits
    bool hasValidOperandFormat = this->hasValidOperandFormat(operandInt);

    // informa si la línea es inválida
    if(usesOperand && !isNumber)
    {
        throw InvalidInstructionException("Operando inválido: " + operandString.toStdString());
    }
    else if (usesOperand && !hasValidOperandFormat)
    {
        throw InvalidInstructionException("Operando fuera de rango: " + operandString.toStdString());
    }
    else if (!usesOperand && !operandString.isEmpty())
    {
        throw InvalidInstructionException("No se esperaba un operando: " + operandString.toStdString());
    }

    inst.opcode = opcodeChar;
    inst.operand = Int24(operandInt);
    return inst;
}

/* Recibe el string del opcode, lo identifica en el mapa de instrucciones y devuelve el char correspondiente */
/* Sí no existe la instrucción devuelve '/0' */
char Assembler::translateOpcodeToChar(const QString& opcodeString)
{
    return InstructionSet.contains(opcodeString) ? InstructionSet.value(opcodeString).opcode : '\0';
}

/* Revisa en el set de instrucciones si la instrucción utiliza un operando */
bool Assembler::usesOperand(const QString& opcodeString)
{
    return InstructionSet.value(opcodeString).usesOperand;
}

/* Comprueba si el opcode tiene tiene formato de signed int 24bits */
bool Assembler::hasValidOperandFormat(int operand)
{
    return (operand > -8388608 && operand < 8388607);
}

/* Guarda el vector de instrucciones en un archivo nuevo, en la ubicación del archivo de texto importado (o creado) */
/* Utiliza el mismo nombre que el archivo importado, con formato .bin */
void Assembler::saveInstructionVectorToBinFile(QVector<Instruction*> &instVector, const QString& outBinFileName)
{
    QFile bin(outBinFileName);
    if (!bin.open(QFile::WriteOnly))
    {
        throw FileException("No se pudo abrir el archivo para escritura.");
    }

    for (int i = 0; i < instVector.size(); i++)
    {
        // Obtiene el opcode y el operando
        char opcode = instVector[i]->opcode;
        int operand = int(instVector[i]->operand);

        // Crea un QByteArray para el operando
        QByteArray operandBytes(reinterpret_cast<char*>(&operand), sizeof(operand));

        // Da vuelta los bytes del operando para big endian
        std::reverse(operandBytes.begin(), operandBytes.end());

        // Sobreescribe el primer byte con el byte de opcode
        operandBytes[0] = opcode;

        // Escribe el QByteArray en el archivo binario
        qint64 bytesWritten = bin.write(operandBytes);

        // Verifica si se escribieron todos los bytes
        if (bytesWritten != operandBytes.size()) {
            throw FileException("Error al escribir en el archivo binario.");
        }

        // Elimina el objeto Instruction después de usarlo
        delete instVector[i];
    }

    bin.close();
}




