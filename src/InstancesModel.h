#ifndef NETLIST_INSTANCESMODEL_H
#define NETLIST_INSTANCESMODEL_H

#include <QAbstractTableModel>

namespace Netlist
{
    class Cell;
    class InstancesModel : public QAbstractTableModel
    {
        Q_OBJECT;

    public:
                                    InstancesModel(QObject *parent = NULL);
                                    ~InstancesModel();
        void                        setCell(Cell *);
                        Cell*       getModel(int row);
        int                         rowCount ( const QModelIndex & parent = QModelIndex ()) const;
        int                         columnCount ( const QModelIndex & parent = QModelIndex ()) const;
                        QVariant    data ( const QModelIndex & index, int role ) const;
                        QVariant    headerData ( int section, Qt :: Orientation orientation, int role = Qt :: DisplayRole ) const ;

    private :
    Cell * cell_ ;
    };

} 

#endif // NETLIST_INSTANCESMODEL_H