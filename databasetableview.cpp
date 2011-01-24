#include "databasetableview.h"
#include "ui_databasetableview.h"
#include <QSqlQuery>
#include <QTextStream>

#define TABLE_NAME "Flaatt"

DatabaseTableView::DatabaseTableView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DatabaseTableView)
{
    this->schema = 0;
    this->data = 0;
    this->dbmFileName = "";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    db.open();

    model = new QSqlTableModel(this);
    model->setTable(TABLE_NAME);

    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setTabKeyNavigation(false);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(model);
    connect(ui->lineEdit, SIGNAL(editingFinished()), this, SLOT(setFilter()));
}

DatabaseTableView::~DatabaseTableView()
{
    delete ui;
}

void DatabaseTableView::readFile(QFile & file)
{
    this->hide();
    this->dbmFileName = file.fileName();
    QSqlQuery query;
    query.exec(QString("DROP TABLE IF EXISTS %1").arg(TABLE_NAME));
    schema = new QProcess(this);
    connect(schema, SIGNAL(finished(int)), this, SLOT(setSchema()));
    schema->start("/usr/bin/mdb-schema", QStringList() << this->dbmFileName << "postgres");
}

void DatabaseTableView::setSchema()
{
    QSqlQuery query;
    QString schemaSql;
    while (!schema->atEnd()) {
        QByteArray raw = schema->readLine();
        if (! raw.contains("DROP")) {
            schemaSql.append(raw);
        }
    }
    query.exec(schemaSql);
    schema->deleteLater();
    schema = 0;
    data = new QProcess(this);
    connect(data, SIGNAL(finished(int)), this, SLOT(setData()));
    data->start("/usr/bin/mdb-export", QStringList() << "-I" << this->dbmFileName << TABLE_NAME);
}

void DatabaseTableView::setData()
{
    QSqlQuery query;
    bool gotData(false);
    while(! data->atEnd())
    {
        query.exec(QString::fromUtf8(data->readLine()));
        gotData = true;
    }
    model->setTable(TABLE_NAME);
    model->select();
    if (gotData) {
        this->show();
    }
    data->deleteLater();
    data = 0;
}

void DatabaseTableView::setFilter ()
{
    model->setFilter(ui->lineEdit->text());
}

void DatabaseTableView::next()
{
    QModelIndexList list(ui->tableView->selectionModel()->selectedIndexes());
    if (list.empty()) {
        ui->tableView->selectRow(0);
    } else {
        ui->tableView->selectRow(list.at(0).row() + 1);
    }
}

void DatabaseTableView::prev()
{
    QModelIndexList list(ui->tableView->selectionModel()->selectedIndexes());
    if (list.empty()) {
        ui->tableView->selectRow(ui->tableView->model()->rowCount() - 1);
    } else {
        ui->tableView->selectRow(list.at(0).row() - 1);
    }
}
