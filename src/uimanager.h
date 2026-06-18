#pragma once
#include <QObject>
#include <QTextEdit>

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

class UIManager {
public:
    static UIManager* getInstance();
    void setTextEdit(QTextEdit* textEdit);
    void appendMessage(const QString& msg);

private:
    UIManager() = default;
    static UIManager* m_instance;
    QTextEdit* m_textEdit = nullptr;
};