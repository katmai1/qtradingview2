#ifndef UIMANAGER_H
#define UIMANAGER_H
#include <QTextEdit>

class UIManager {
public:
    static UIManager *getInstance() {
        static UIManager instance;
        return &instance;
    }

    QTextEdit *getTextEdit() const { return textEdit; }
    void setTextEdit(QTextEdit *txtedit) { textEdit = txtedit; }

private:
    UIManager() : textEdit(nullptr) {} // Initialize textEdit to nullptr

    QTextEdit *textEdit;
};
#endif // UIMANAGER_H
