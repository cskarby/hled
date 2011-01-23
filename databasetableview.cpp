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
    this->model = 0;
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(setFilter(QString)));
}

DatabaseTableView::~DatabaseTableView()
{
    delete ui;
}

void DatabaseTableView::setModel ( QSqlTableModel * model )
{
    if (model != 0) {
        this->model = model;
        ui->tableView->setModel(this->model);
        this->setFilter(ui->lineEdit->text());
    }
}

void DatabaseTableView::setFilter (const QString &filter)
{
    if (model != 0) {
        model->setFilter(filter);
    }
}
