#include "uimanager.h"
#include "qdatetime.h"
#include "qscrollbar.h"

UIManager* UIManager::m_instance = nullptr;

UIManager* UIManager::getInstance() {
    if (!m_instance)
        m_instance = new UIManager();
    return m_instance;
}

void UIManager::setTextEdit(QTextEdit* textEdit) {
    m_textEdit = textEdit;
}

void UIManager::appendMessage(const QString& msg) {
    if (!m_textEdit) return;
    m_textEdit->append(msg);
    // Auto-scroll al final
    m_textEdit->verticalScrollBar()->setValue(
        m_textEdit->verticalScrollBar()->maximum()
        );
}

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    // METER ESTO EN FUNCION APARTE
    // Filtrar mensajes de Google Ads y otros ruidos del webengine
    const QStringList filters = {
        "GPT",
        "ad request",
        "goo.gle",
        "TCF signal",
        "Autofill.enable",
        "webchannel"
    };

    for (const auto& filter : filters)
        if (msg.contains(filter, Qt::CaseInsensitive))
            return;  // ignorar

    QString color;
    QString prefix;

    switch (type) {
    case QtDebugMsg:    prefix = "[DEBUG]";   color = "gray";   break;
    case QtInfoMsg:     prefix = "[INFO]";    color = "green";  break;
    case QtWarningMsg:  prefix = "[WARNING]"; color = "orange"; break;
    case QtCriticalMsg: prefix = "[ERROR]";   color = "red";    break;
    case QtFatalMsg:    prefix = "[FATAL]";   color = "red";    break;
    }

    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

    QString formatted = QString("<span style='color:gray'>%1</span> <span style='color:%2'>%3</span> %4")
                            .arg(timestamp, color, prefix, msg.toHtmlEscaped());

    fprintf(stderr, "%s %s %s\n",
            timestamp.toLocal8Bit().constData(),
            prefix.toLocal8Bit().constData(),
            msg.toLocal8Bit().constData());

    UIManager::getInstance()->appendMessage(formatted);
}
