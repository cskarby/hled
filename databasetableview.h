#ifndef DATABASETABLEVIEW_H
#define DATABASETABLEVIEW_H

#include <QDockWidget>
#include <QSqlTableModel>

namespace Ui {
    class DatabaseTableView;
}

class DatabaseTableView : public QDockWidget
{
    Q_OBJECT

public:
    explicit DatabaseTableView(QWidget *parent = 0);
    ~DatabaseTableView();

    void setModel ( QSqlTableModel * /* model */ );

private:
    Ui::DatabaseTableView *ui;
    QSqlTableModel * model;

private slots:
    void setFilter(const QString & filter);
};

#endif // DATABASETABLEVIEW_H
