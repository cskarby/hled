/****************************************************************************
**
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/
#ifndef URLSHOWER_H
#define URLSHOWER_H

#include <QObject>
#include <QUrl>

class UrlShower : public QObject
{
Q_OBJECT
public:
    UrlShower(const QUrl & url, QObject *parent = 0);
public slots:
    void show();
private:
    QUrl url;
};

#endif // URLSHOWER_H
