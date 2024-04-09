#ifndef QTEXTEDITLOGGER_H
#define QTEXTEDITLOGGER_H
#include <QTextEdit>
#include <QMetaEnum>
#include <QDebug>

class QTextEditLogger : public QObject
{
    Q_OBJECT

public:
    explicit QTextEditLogger(CustomTextEdit* customTextEdit, QObject* parent = nullptr)
        : QObject(parent)
        , m_customTextEdit(customTextEdit)
    {
        qInstallMessageHandler(handleMessage);
    }

    ~QTextEditLogger()
    {
        qInstallMessageHandler(nullptr);
    }

private:
    CustomTextEdit* m_customTextEdit;

    static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        // This is now a non-static member function, able to access the member variables
        m_customTextEdit->customAppend("Your message here");
    }

    void customAppend(const QString& text)
    {
        if (m_customTextEdit)
        {
            m_customTextEdit->append(text);
        }
    }
};


#endif // QTEXTEDITLOGGER_H
