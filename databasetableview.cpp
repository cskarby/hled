#include "databasetableview.h"
#include "ui_databasetableview.h"
#include <QBuffer>
#include <QRegExp>
#include <QSqlQuery>
#include <QTextStream>

#define TABLE_NAME "Flaatt"
#define NOTE_TABLE "NOTAT"

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
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection)));
}

DatabaseTableView::~DatabaseTableView()
{
    delete ui;
}

void DatabaseTableView::readFile(QFile & file, QByteArray buf)
{
    this->hide();
    this->dbmFileName = file.fileName();
    QSqlQuery query;
    query.exec(QString("DROP TABLE IF EXISTS %1").arg(TABLE_NAME));
    schema = new QProcess(this);
    connect(schema, SIGNAL(finished(int)), this, SLOT(setSchema()));
    schema->start("/usr/bin/mdb-schema", QStringList() << this->dbmFileName << "postgres");

    query.exec(QString("DROP TABLE IF EXISTS %1").arg(NOTE_TABLE));
    query.exec(QString("CREATE TABLE %1 (key1 Char(14), key2 Int8, key3 Timestamp, data Text)").arg(NOTE_TABLE));
    query.prepare(QString("INSERT INTO %1 (key1, key2, key3, data) VALUES (:key1, :key2, :key3, :data)").arg(NOTE_TABLE));
    QByteArray alarm("\a");
    buf.replace(alarm, QByteArray("\r"));
    buf.replace("-------------------------------------------------------\n-------------------------------------------------------\n", alarm);
    QByteArray note;
    QBuffer tmp;
    QRegExp re1(":\\s+(\\d+)");
    QRegExp re2(":\\s+(\\d+\\.\\d+\\.\\d+)");
    QString line;
    foreach(note, buf.split('\a')) {
        tmp.close();
        tmp.setData(note);
        tmp.open(QBuffer::ReadOnly);
        line = QString::fromUtf8(tmp.readLine());
        if (re1.indexIn(line, 0) != -1)
            query.bindValue(":key1", re1.cap(1));
        if (re2.indexIn(line, 0) != -1)
            query.bindValue(":key3", re2.cap(1));
        line = QString::fromUtf8(tmp.readLine());
        if (re1.indexIn(line, 0) != -1)
            query.bindValue(":key2", re1.cap(1));
        query.bindValue(":data", note);
        query.exec();
    }
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
    data->start("/usr/bin/mdb-export", QStringList() << "-D" << "%d.%m.%Y" << "-I" << this->dbmFileName << TABLE_NAME);
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

void DatabaseTableView::selectionChanged(const QItemSelection &selected)
{
    QModelIndex row(selected.indexes().at(0));
    QSqlQuery query;
    query.prepare(QString("SELECT data FROM %1 WHERE key1=:key1 AND key2=:key2 AND key3=:key3").arg(NOTE_TABLE));
    query.bindValue(":key1", model->data(row.sibling(row.row(), 0)).toString());
    query.bindValue(":key2", model->data(row.sibling(row.row(), 1)).toString());
    query.bindValue(":key3", model->data(row.sibling(row.row(), 5)).toString());
    query.exec();
    emit plainText(model->data(row.sibling(row.row(), 5)).toString());
    if (query.next()) {
        emit plainText(query.value(0).toString());
    }
}
