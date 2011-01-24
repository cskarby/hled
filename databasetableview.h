#ifndef DATABASETABLEVIEW_H
#define DATABASETABLEVIEW_H

#include <QDockWidget>
#include <QFile>
#include <QItemSelection>
#include <QSqlTableModel>
#include <QProcess>

namespace Ui {
    class DatabaseTableView;
}

class DatabaseTableView : public QDockWidget
{
    Q_OBJECT

public:
    explicit DatabaseTableView(QWidget *parent = 0);
    ~DatabaseTableView();

    void readFile(QFile & /* file */, QByteArray buf);

public slots:
    void next();
    void prev();

signals:
    void plainText(const QString & text);

private:
    Ui::DatabaseTableView *ui;
    QSqlTableModel * model;
    QProcess * schema;
    QProcess * data;
    QString dbmFileName;

private slots:
    void setFilter();
    void setSchema();
    void setData();
    void selectionChanged ( const QItemSelection & selected);
};

#endif // DATABASETABLEVIEW_H
