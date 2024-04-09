// qtexteditdevice.cpp
#include "qtexteditdevice.h"

QTextEditDevice::QTextEditDevice(QTextEdit *textEdit) : QTextStream(&buffer), m_textEdit(textEdit) {}

void QTextEditDevice::write(const QString &string)
{
    m_textEdit->moveCursor(QTextCursor::End);
    *this << string;
}
