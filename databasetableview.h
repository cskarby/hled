#ifndef DATABASETABLEVIEW_H
#define DATABASETABLEVIEW_H

#include <QDockWidget>

namespace Ui {
    class DatabaseTableView;
}

class DatabaseTableView : public QDockWidget
{
    Q_OBJECT

public:
    explicit DatabaseTableView(QWidget *parent = 0);
    ~DatabaseTableView();

private:
    Ui::DatabaseTableView *ui;
};

#endif // DATABASETABLEVIEW_H
