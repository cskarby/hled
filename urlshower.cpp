/****************************************************************************
**
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/
#include <QDesktopServices>
#include "urlshower.h"

UrlShower::UrlShower(const QUrl & url, QObject *parent) :
    QObject(parent)
{
    this->url = url;
}

void UrlShower::show()
{
     QDesktopServices::openUrl(this->url);
}
