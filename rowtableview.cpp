#include "rowtableview.h"

RowTableView::RowTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setTabKeyNavigation(false);
}
