/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/


#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    format.setForeground(Qt::red);
    format.setFontWeight(QFont::Bold);
    matchWords = 0;
}

void Highlighter::highlightBlock(const QString &text, const QString &pattern, const QTextCharFormat & highlightFormat)
{
    QRegExp re(pattern, Qt::CaseInsensitive);
    int index = re.indexIn(text);
    while (index >= 0) {
        int length = re.matchedLength();
        setFormat(index, length, highlightFormat);
        index = re.indexIn(text, index + length);
    }
}

void Highlighter::highlightBlock(const QString &text)
{
    if (matchWords != 0) {
        QStringList stringList = matchWords->stringList();
        foreach (const QString &pattern, stringList) {
            highlightBlock(text, pattern, format);
        }

        // Tone down likely RTF-encoding
        QTextCharFormat codeFormat;
        codeFormat.setForeground(Qt::lightGray);
        highlightBlock(text, "\\\\[a-z0-9\\\\]+", codeFormat);
        highlightBlock(text, "\\{.*\\}", codeFormat);
        highlightBlock(text, "\\}", codeFormat);
    }
}

void Highlighter::setModel(QStringListModel * newList)
{
    if (matchWords != 0) {
        matchWords->disconnect(this);
    }
    matchWords = newList;
    connect(matchWords, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(rehighlight()));
}

void Highlighter::setColor(const QColor &color)
{
    format.setForeground(color);
    rehighlight();
}
