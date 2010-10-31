/****************************************************************************
**
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/
#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include "ui_about.h"

class About : public QWidget
{
    Q_OBJECT

public:
    About(const QString &title, const QString &fileName, QWidget *parent = 0);

private:
    Ui::Form ui;
};

#endif // ABOUT_H
