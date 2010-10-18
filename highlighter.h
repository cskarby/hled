/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/


#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QColor;
class QStringListModel;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

public slots:
    void setModel(QStringListModel * matchWords);
    void setColor(const QColor & color);

protected:
    void highlightBlock(const QString &text);
    void highlightBlock(const QString &text, const QString &pattern, const QTextCharFormat & highlightFormat);

private:
    QStringListModel * matchWords;
    QTextCharFormat format;
};

#endif
