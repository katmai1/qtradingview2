#include <QDebug>
#include <QTextEdit>

class QTextEditLogger : public QObject
{
    Q_OBJECT
public:
    explicit QTextEditLogger(QTextEdit *textEdit, QObject *parent = nullptr) :
        QObject(parent), m_textEdit(textEdit)
    {
        // Conectar la señal de `qDebug()` a nuestra ranura personalizada
        qInstallMessageHandler(QTextEditLogger::handleMessage);
    }

    // Esta es nuestra ranura personalizada para manejar los mensajes de depuración
    static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        Q_UNUSED(type);
        Q_UNUSED(context);

        // Agregar el mensaje al QTextEdit
        m_textEdit->append(msg);
    }

private:
    QTextEdit *m_textEdit;
};

