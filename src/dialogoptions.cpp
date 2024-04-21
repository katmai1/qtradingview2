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

    //config settings;
    SettingsManager settings;

    this->addLanguages();

    // seleccion el idioma configurado en el combo
    for(int i = 0; i < this->ui->comboLanguage->count(); ++i) {
        ComboItem item = this->ui->comboLanguage->itemData(i).value<ComboItem>();
        if (item.extraData == settings.language()) {
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
    QString oldLanguage = settings.language();
    if (newLanguage != oldLanguage) {
        settings.setValue("language", newLanguage);
        QMessageBox::information(nullptr, "Info", "El idioma seleccionado se cargará al reiniciar la aplicación");
    }
}

// carga idiomas en el combo
void dialogoptions::addLanguages()
{
    ComboItem system = {"Sistema", "system"};
    ComboItem english = {"Inglés", "en_EN"};
    ComboItem spanish = {"Español", "es_ES"};
    ComboItem french = {"Francés", "fr_FR"};
    ComboItem italiano = {"Italiano", "it_IT"};
    ComboItem catalan = {"Catalan", "ca_ES"};
    this->ui->comboLanguage->addItem(system.name, QVariant::fromValue(system));
    this->ui->comboLanguage->addItem(english.name, QVariant::fromValue(english));
    this->ui->comboLanguage->addItem(spanish.name, QVariant::fromValue(spanish));
    this->ui->comboLanguage->addItem(french.name, QVariant::fromValue(french));
    this->ui->comboLanguage->addItem(italiano.name, QVariant::fromValue(italiano));
    this->ui->comboLanguage->addItem(catalan.name, QVariant::fromValue(catalan));
}
