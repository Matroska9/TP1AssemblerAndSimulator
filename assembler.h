#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instruction.h"
#include "exceptions.h"
#include <QException>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QScrollBar>
#include <QTextBlock>
#include <QDebug>
#include <QVector>
#include <QFile>

class Assembler
{
private:
    Instruction readInstructionFromLine(const QString& inTxt);
    QString createBinFileName(const QString& inTxtFileName);
    char translateOpcodeToChar(const QString& opcodeString);
    bool usesOperand(const QString& opcodeString);
    bool hasValidOperandFormat(int operand);
    void saveInstructionVectorToBinFile(QVector<Instruction*> &instVector, const QString& outBinFileName);

public:
    Assembler();
    QString generateBin(QString inTxt, QString inTxtFileName);

};

#endif // ASSEMBLER_H
