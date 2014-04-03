//
//  itemmodel.h
//  Fake320
//
//  Created by Vincent Timofti on 03/04/2014.
//
//

#ifndef __Fake320__itemmodel__
#define __Fake320__itemmodel__

#include <iostream>
#include <QStandardItemModel>

#define COLUMNS_NUMBER 4

class ItemModel : public QStandardItemModel
{
public:
    ItemModel();
    void addItem(int, std::string);
    
private:

    
};


#endif /* defined(__Fake320__itemmodel__) */
