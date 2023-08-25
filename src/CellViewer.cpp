#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDialog>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "CellViewer.h"
#include "Cell.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
#include "InstancesWidget.h"
#include "CellsLib.h"

namespace Netlist
{
    using namespace std;

    CellViewer::CellViewer(QWidget *parent)
        : QMainWindow(parent),
          cellWidget_(NULL),
          saveCellDialog_(NULL),
          instancesWidget_(NULL),
          cellsLib_(NULL)

    {
        cellWidget_ = new CellWidget();
        saveCellDialog_ = new SaveCellDialog(this);
        setCentralWidget(cellWidget_);
        QMenu *fileMenu = menuBar()->addMenu("&File");
        QAction *action = new QAction("&Save as", this);

        // Sauvegarde
        action->setStatusTip("save to disk");
        action->setShortcut(QKeySequence("CTRL+S"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));

        cellsLib_ = new CellsLib(NULL);
        cellsLib_->setCellViewer(this);
        // Fermeture
        action = new QAction("&Quit", this);
        action->setStatusTip("quitter l'APP");
        action->setShortcut(QKeySequence("CTRL+Q"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(close()));
        connect(this, SIGNAL(cellLoaded()), cellsLib_->getBaseModel(), SLOT(updateDatas()));

        // Ouverture Cell
        action = new QAction("&Open Cell", this);
        action->setStatusTip("ouverture de la Cell");
        action->setShortcut(QKeySequence("CTRL+O"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(openCell()));

        instancesWidget_ = new InstancesWidget(NULL);
        instancesWidget_->setCellViewer(this);
        // Ouverture Instance Widget
        action = new QAction("&Open Instance Widget", this);
        action->setStatusTip("ouverture de l'Instance");
        action->setShortcut(QKeySequence("CTRL+I"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));


        // Ouverture Instance Widget
        action = new QAction("&Open Cell Widget", this);
        action->setStatusTip("ouverture de la cell");
        action->setShortcut(QKeySequence("CTRL+M"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));
    }

    CellViewer::~CellViewer()
    {
        delete cellWidget_;
    }

    Cell *CellViewer::getCell() const
    {
        return cellWidget_->getCell();
    }

    void CellViewer::setCell(Cell *c)
    {
        cellWidget_->setCell(c);

        instancesWidget_->setCell(c);

        cellsLib_->getBaseModel()->setCell(c);
    }

    void CellViewer::saveCell()
    {
        Cell *cell = getCell();
        if (not cell)
            return;
        QString cellName = cell->getName().c_str();
        if (saveCellDialog_->run(cellName))
        {
            cell->setName(cellName.toStdString());
            cell->save(cellName.toStdString());
        }
    }
    void CellViewer::openCell()
    {
        QString name;
        Cell *cell; // La cell sera crée si elle n'existe pas encore

        if (OpenCellDialog::run(name)) // L'ouverture est accepté dans OpenCellDialog::run
        {
            if (Cell::find(name.toStdString()))
            { // La cell existe déjà
                cout << "La cell existe déjà" << endl;
            }

            if (not(Cell::find(name.toStdString())))
            { // La cell n'existe pas, il faut la charger
                cell = Cell::load(name.toStdString());
                cell->setName(name.toStdString());
                setCell(cell);
                emit cellLoaded();
            }
        }
        else
        {
            cout << "L'ouverture a été décliné dans OpenCellDialog::run";
        }
    }

    void CellViewer::showCellsLib()
    {
        cellsLib_->show();
    } 

    void CellViewer::showInstancesWidget()
    {
        instancesWidget_->show();
    }

}