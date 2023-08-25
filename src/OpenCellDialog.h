#ifndef NETLIST_OPENCELLDIALOG_H
#define NETLIST_OPENCELLDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Netlist{
    
    class OpenCellDialog : public QDialog {
    Q_OBJECT ;
    public :
                                OpenCellDialog ( QWidget * parent = NULL ); //incomplet
        static bool             run ( QString & name ); //static car on crée un OpenCellDialog à la volée
        const       QString     getCellName () const ;
        void                    setCellName ( const QString & );
    protected :
        QLineEdit * lineEdit_ ;
    };
}


#endif  // NETLIST_OPENCELLDIALOG_H