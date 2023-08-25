
#ifndef NETLIST_CELLSLIB_H
#define NETLIST_CELLSLIB_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>

#include <iostream>


namespace Netlist{
    class CellViewer;
    class CellsModel;
    class Cell;

    class CellsLib : public QWidget {
        Q_OBJECT;
    public:
                                    CellsLib ( QWidget* parent=NULL );
                void                setCellViewer   ( CellViewer* );
                int                 getSelectedRow  () const;
                inline CellsModel*  getBaseModel   ();
    public slots:
                void                load();
    private:
                CellViewer*     cellViewer_;
                CellsModel*     baseModel_;
                QTableView*     view_;
                QPushButton*    load_;
    };

    inline CellsModel*  CellsLib::getBaseModel   (){
        if(baseModel_ != NULL){
            return baseModel_;
        }else{std::cout << "baseModel_ NULL" << std::endl;}    
    }
} // Netlist namespace.



#endif  // NETLIST_CELLSLIB_H