#include <QFile>
#include "about.h"

About::About(const QString &title, const QString &fileName, QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(title);
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        ui.textBrowser->setPlainText(file.readAll());
    }
    ui.label->setText(qApp->applicationName() + " " + qApp->applicationVersion());
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(hide()));
}
