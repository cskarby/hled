/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAINWINDOW_SRC "$HeadURL$"

#include <QMainWindow>

#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QColorDialog;
class QStringListModel;
class QListView;
class QAction;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void newFile();
    void openFile(const QString &path = QString());
    void chooseFont();
    void addSearchTermDialog();
    void removeSearchTerm();
    void openRecentFile();
    void find();
    void findNext();
    void findPrev();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void setupSearchWordsWidget();
    void setupFileMenu();
    void setupViewMenu();
    void setupHelpMenu();
    void readSettings();
    void writeSettings();
    void updateRecentFiles(const QString & filePath = "");
    const QString strippedName(const QString &);

    int MaxRecentFiles;

    QTextEdit *editor;
    Highlighter *highlighter;
    QStringListModel * matchWords;
    QDockWidget * searchWordsWidget;
    QColorDialog * matchedColor;
    QListView * listView;
    QAction * separatorAct;
    QAction * recentFileActs[10];
    QString findExp;
};
//! [0]

#endif
