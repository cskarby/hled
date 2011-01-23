#ifndef DATABASETABLEVIEW_H
#define DATABASETABLEVIEW_H

#include <QDockWidget>
#include <QFile>
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

    void readFile ( QFile & /* file */);

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
};

#endif // DATABASETABLEVIEW_H
