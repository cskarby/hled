/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Interhost AS");
    app.setOrganizationDomain("interhost.no");
    app.setApplicationName("HLED");
    MainWindow window;
    window.show();
    return app.exec();
}
