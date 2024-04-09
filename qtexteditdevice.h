#ifndef QTEXTEDITDEVICE_H
#define QTEXTEDITDEVICE_H

#include <QTextStream>
#include <QTextEdit>

class QTextEditDevice : public QTextStream
{
public:
    explicit QTextEditDevice(QTextEdit *textEdit);

protected:
    virtual void write(const QString &string);

private:
    QTextEdit *m_textEdit;
    QString buffer;
};

#endif // QTEXTEDITDEVICE_H
