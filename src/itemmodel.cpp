//
//  itemmodel.cpp
//  Fake320
//
//  Created by Vincent Timofti on 03/04/2014.
//
//

#include "itemmodel.h"

ItemModel::ItemModel() : QStandardItemModel(0, COLUMNS_NUMBER)
{
    this->setHorizontalHeaderItem(1, new QStandardItem("Filename"));
    this->setHorizontalHeaderItem(2, new QStandardItem("Cut-off frequency"));
    this->setHorizontalHeaderItem(3, new QStandardItem("Rate"));
}

void ItemModel::addItem(int id, std::string filename) {

    
    QList<QStandardItem *> list;
    
    list.append(new QStandardItem(id));
    list.append(new QStandardItem(filename.c_str()));
    
    this->insertRow(id, list);
}

