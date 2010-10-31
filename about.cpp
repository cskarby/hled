#include <QFile>
#include "about.h"

About::About(const QString &title, const QString &fileName, QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(title);
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        ui.textBrowser->setPlainText(QString::fromUtf8(file.readAll()));
    }
    ui.label->setText(qApp->applicationName() + " " + qApp->applicationVersion() + " created by Interhost AS - www.interhost.no - +47  21 42 01 12.");
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(hide()));
}
