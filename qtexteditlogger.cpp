#include "qtexteditlogger.h"

QTextEditLogger::QTextEditLogger(QTextEdit *textEdit, QObject *parent) :
    QObject(parent), m_textEdit(textEdit)
{
    // Conectamos la señal de `qDebug()` a nuestra ranura showMessage()
    qInstallMessageHandler(handleMessage);
}

void QTextEditLogger::showMessage(const QString &message)
{
    m_textEdit->append(message);
}

void QTextEditLogger::handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);

    QTextEditLogger *logger = static_cast<QTextEditLogger *>(QObject::sender());
    if (logger)
        logger->showMessage(msg);
}
