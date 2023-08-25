#ifndef NETLIST_SAVECELLDIALOG_H
#define NETLIST_SAVECELLDIALOG_H

#include <QDialog>
#include <QLineEdit>


namespace Netlist{
    class SaveCellDialog : public QDialog
    {
        Q_OBJECT;

    public:
                        SaveCellDialog(QWidget *parent = NULL);
        bool            run(QString &name);
        const QString   getCellName() const;
        void            setCellName(const QString &);

    protected:
        QLineEdit *lineEdit_;
    };
} // Netlist namespace.

#endif  // NETLIST_SAVECELLDIALOG_H