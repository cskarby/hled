/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2010 Interhost AS - www.interhost.no
** Distributed under the terms of the GNU General Public License v3
**
****************************************************************************/

#include <QtGui>

#include "about.h"
#include "databasetableview.h"
#include "mainwindow.h"
#include "urlshower.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/qrc/hled.png"));
    MaxRecentFiles = 5;
    editor = new QTextEdit;
    highlighter = new Highlighter(editor->document());
    setupFileMenu();
    setupSearchWordsWidget();
    setupDatabaseWidget();
    setupViewMenu();
    setupHelpMenu();
    setCentralWidget(editor);
    addDockWidget(Qt::RightDockWidgetArea, searchWordsWidget);
    addDockWidget(Qt::TopDockWidgetArea, databaseWidget);
    newFile();
    updateRecentFiles();
    readSettings();
}

void MainWindow::newFile()
{
    editor->clear();
    setWindowTitle(qApp->applicationName());
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;
    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), "");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QByteArray buf(file.readAll());
            // avoid truncated view ending on first occurance of NULL
            buf.replace('\0', '\r');
            editor->setPlainText(buf);
            QString title(qApp->applicationName());
            title.append(": ");
            title.append(strippedName(fileName));
            setWindowTitle(title);
            updateRecentFiles(fileName);
        } else {
            newFile();
        }
    }
}

void MainWindow::updateRecentFiles(const QString & filePath)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    QStringList recentFiles = settings.value("recentFiles").toStringList();

    if (! filePath.isEmpty()) {
        recentFiles.removeAll(filePath);
        recentFiles.prepend(filePath);
        while (recentFiles.count() > MaxRecentFiles) {
            recentFiles.removeLast();
        }
        settings.setValue("recentFiles", recentFiles);
    }

    int numRecentFiles = qMin(recentFiles.size(), MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(recentFiles.at(i)));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

const QString MainWindow::strippedName(const QString & filePath)
{
    QFileInfo orig(filePath);
    QString canonical = orig.canonicalFilePath();
    QStringList list(canonical.split("/"));
    int n = qMin(list.size(), 3);
    while (list.size() > n) {
        list.removeFirst();
    }
    return list.join("/");
}

void MainWindow::setupSearchWordsWidget()
{
    searchWordsWidget = new QDockWidget("Search terms");
    searchWordsWidget->setObjectName("searchWordsWidget"); // for saveState()
    matchWords = new QStringListModel();
    matchedColor = new QColorDialog;

    QPushButton * fontColorButton = new QPushButton; fontColorButton->setText("Color");
    QPushButton * addButton = new QPushButton; addButton->setText("Add term");
    QPushButton * removeButton = new QPushButton; removeButton->setText("Remove term");

    QWidget * inner = new QWidget;
    QVBoxLayout * layout = new QVBoxLayout;
    listView = new QListView;
    listView->setModel(matchWords);
    highlighter->setModel(matchWords);
    layout->addWidget(listView);
    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(fontColorButton);
    inner->setLayout(layout);
    searchWordsWidget->setWidget(inner);

    connect(fontColorButton, SIGNAL(clicked()), matchedColor, SLOT(open()));
    connect(matchedColor, SIGNAL(colorSelected(QColor)), highlighter, SLOT(setColor(QColor)));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addSearchTermDialog()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeSearchTerm()));
}

void MainWindow::setupDatabaseWidget()
{
    databaseWidget = new DatabaseTableView();
    databaseWidget->setObjectName("databaseWidget"); // for saveState()
}

void MainWindow::addSearchTermDialog()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add search term"), tr("Search term:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        QStringList copy(matchWords->stringList());
        copy.append(text);
        copy.sort();
        matchWords->setStringList(copy);
        highlighter->rehighlight();
    }
}

void MainWindow::removeSearchTerm()
{
    matchWords->removeRow(listView->currentIndex().row());
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this, SLOT(newFile()),
                        QKeySequence::New);

    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                        QKeySequence::Open);

    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
            recentFileActs[i] = new QAction(this);
            recentFileActs[i]->setVisible(false);
            connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
                        
    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()),
                        QKeySequence::Quit);

    QMenu * editMenu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(editMenu);
    editMenu->addAction(tr("&Find"), this, SLOT(find()), QKeySequence::Find);
    editMenu->addAction(tr("Find &next"), this, SLOT(findNext()), QKeySequence::FindNext);
    editMenu->addAction(tr("Find &previous"), this, SLOT(findPrev()), QKeySequence::FindPrevious);
    editMenu->addSeparator();
    editMenu->addAction(tr("N&ext note"), this, SLOT(findNext()), QKeySequence::NextChild);
    editMenu->addAction(tr("P&revious note"), this, SLOT(findPrev()), QKeySequence::PreviousChild);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);
    QWidget *help = new About("Help", ":/qrc/HELP");
    helpMenu->addAction(tr("&Help"), help, SLOT(show()), QKeySequence::HelpContents);
    UrlShower * projectWiki = new UrlShower(QUrl("https://dev.interhost.no/hled/"));
    helpMenu->addAction(tr("Project &website"), projectWiki, SLOT(show()));
    UrlShower * qregexp = new UrlShower(QUrl("http://doc.qt.nokia.com/latest/qregexp.html"));
    helpMenu->addAction(tr("Regular expressions (technical web site)"), qregexp, SLOT(show()));
    QString title("About ");
    QWidget *about = new About(title + qApp->applicationName(), ":/qrc/README");
    helpMenu->addAction(tr("&About"), about, SLOT(show()));
    title = "License for ";
    QWidget *license = new About(title + qApp->applicationName(), ":/qrc/LICENSE");
    helpMenu->addAction(tr("&License"), license, SLOT(show()));
}

void MainWindow::find()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Jump to"), tr("Find:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        findExp = text;
        findNext();
    }
}

void MainWindow::findNext()
{
    editor->find(findExp);
}

void MainWindow::findPrev()
{
    editor->find(findExp, QTextDocument::FindBackward);
}

void MainWindow::openRecentFile()
{
    QAction * action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString());
}

void MainWindow::setupViewMenu()
{
    QMenu *viewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(viewMenu);
    viewMenu->addAction(tr("&Font"), this, SLOT(chooseFont()));
    viewMenu->addAction(tr("&Search terms"), searchWordsWidget, SLOT(show()));
    viewMenu->addAction(tr("&Database"), databaseWidget, SLOT(show()));
}

void MainWindow::chooseFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, editor->font(), this);
    if (ok) {
        editor->setFont(font);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    editor->setFont(settings.value("font", QFont("Verdana", 10)).value<QFont>());
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    matchedColor->setCurrentColor(settings.value("color", QColor(Qt::red)).value<QColor>());
    highlighter->setColor(matchedColor->currentColor());

    QVariant searchTerms(settings.value("searchTerms"));
    if (!searchTerms.isValid()) {
        QStringList list;
        list << "erytem"
             << "erythem"
             << "\\bfl.t"
             << "\\bskaumann\\b"
             << "\\bmigrans\\b"
             << "\\blyme\\b"
             << "bor(|r)el"
             << "bitt\\b"
             << "stikk\\b"
             << "\\bstukket\\b"
             << "\\be(c|)m\\b"
        ;
        list.sort();
        searchTerms = QVariant(list);
    }
    matchWords->setStringList(searchTerms.value<QStringList>());
    highlighter->rehighlight();

    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("font", editor->font());
    settings.setValue("color", matchedColor->currentColor());
    settings.setValue("searchTerms", QVariant(matchWords->stringList()));
    settings.endGroup();
}
