
#ifndef NETLIST_CELLVIEWER_H
#define NETLIST_CELLVIEWER_H

#include <QMainWindow>
#include <QWidget>

#include "CellWidget.h"
#include "Cell.h"
#include "CellsLib.h"
#include "CellsModel.h"

namespace Netlist{
    class Cell;
    class SaveCellDialog;
    class CellWidget;
    class InstancesWidget;


    class CellViewer : public QMainWindow {
        Q_OBJECT;
    public:
                            CellViewer          ( QWidget* parent = NULL );
        virtual             ~CellViewer          ();
                Cell*       getCell             () const;
        inline  CellsLib*   getCellsLib         (); 
    public slots:
                void        setCell             ( Cell* );
                void        saveCell            ();
                void        openCell            ();
                void        showCellsLib        (); 
                void        showInstancesWidget (); 
    signals:
                void        cellLoaded(); // méthode sans corps (son implantation est générée par moc)
    private:
        CellWidget*      cellWidget_;
        CellsLib*        cellsLib_;        
        InstancesWidget* instancesWidget_; 
        SaveCellDialog*  saveCellDialog_;
    };

    inline  CellsLib* CellViewer::getCellsLib         (){return cellsLib_;}  

} // Netlist namespace.



#endif  // NETLIST_CELLVIEWER_H