#include "databasetableview.h"
#include "ui_databasetableview.h"

DatabaseTableView::DatabaseTableView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DatabaseTableView)
{
    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setTabKeyNavigation(false);
}

DatabaseTableView::~DatabaseTableView()
{
    delete ui;
}
