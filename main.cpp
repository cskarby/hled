/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/

#include <QApplication>
#include <QString>
#include <QFileInfo>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Interhost AS");
    app.setOrganizationDomain("interhost.no");
    app.setApplicationName("HLED");
    QString mainwindow_src(MAINWINDOW_SRC);
    mainwindow_src.remove(QRegExp("HeadURL:?"));
    mainwindow_src.remove("$");
    QFileInfo path(mainwindow_src);
    app.setApplicationVersion(path.dir().dirName());
    MainWindow window;
    window.show();
    return app.exec();
}
