#include "dialogoptions.h"
#include "mainwindow.h"
#include "ui_dialogoptions.h"
#include <QComboBox>
#include <QMessageBox>
#include "src/settings.h"


struct ComboItem {
    QString name;
    QString extraData;
};

dialogoptions::dialogoptions(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::dialogoptions),
    mwin(parent)
{
    ui->setupUi(this);

    this->addLanguages();

    // seleccion el idioma configurado en el combo
    for(int i = 0; i < this->ui->comboLanguage->count(); ++i) {
        ComboItem item = this->ui->comboLanguage->itemData(i).value<ComboItem>();
        if (item.extraData == SettingsManager::getInstance().language()) {
            this->ui->comboLanguage->setCurrentIndex(i);
        }
    }
}

dialogoptions::~dialogoptions()
{
    delete ui;
}

void dialogoptions::on_buttonBox_accepted()
{
    // obtiene idioma (ex: ca_ES)
    int currentIndex = this->ui->comboLanguage->currentIndex();
    ComboItem currentItem = this->ui->comboLanguage->itemData(currentIndex).value<ComboItem>();
    QString newLanguage = currentItem.extraData;

    // compara con el anterior y si es diferent lo guarda
    QString oldLanguage = SettingsManager::getInstance().language();
    if (newLanguage != oldLanguage) {
        SettingsManager::getInstance().setValue("language", newLanguage);
        QMessageBox::information(nullptr, "Info", "El idioma seleccionado se cargará al reiniciar la aplicación");
    }
}

// carga idiomas en el combo
void dialogoptions::addLanguages()
{
    // añade opcion sistema
    ComboItem system = {"Sistema", "system"};
    this->ui->comboLanguage->addItem(system.name, QVariant::fromValue(system));
    // carga lista de idiomas disponibles y los añade
    QList<Languages> languages = SettingsManager::getInstance().availableLanguages();
    foreach (const Languages &lang, languages) {
        ComboItem idioma = {lang.name, lang.locale};
        this->ui->comboLanguage->addItem(idioma.name, QVariant::fromValue(idioma));

    }
}
