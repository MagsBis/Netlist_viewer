#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "CellsLib.h"
#include "CellsModel.h"
#include "CellViewer.h"

namespace Netlist{

    CellsLib::CellsLib ( QWidget* parent)
        : QWidget(parent),
          cellViewer_(NULL),
          baseModel_(new CellsModel(this)),
          view_(new QTableView(this)),
          load_(new QPushButton(this)){
        setAttribute(Qt ::WA_QuitOnClose, false);
        setAttribute(Qt ::WA_DeleteOnClose, false);
        setContextMenuPolicy(Qt ::ActionsContextMenu);
        view_->setShowGrid(false);
        view_->setAlternatingRowColors(true);
        view_->setSelectionBehavior(QAbstractItemView ::SelectRows);
        view_->setSelectionMode(QAbstractItemView ::SingleSelection);
        view_->setSortingEnabled(true);
        view_->setModel(baseModel_); // On associe le modele
        QHeaderView *horizontalHeader = view_ -> horizontalHeader();
        horizontalHeader->setDefaultAlignment(Qt ::AlignHCenter);
        horizontalHeader->setMinimumSectionSize(300);
        horizontalHeader->setStretchLastSection(true);
        QHeaderView *verticalHeader = view_->verticalHeader();
        verticalHeader->setVisible(false);
        load_->setText(" Load ");
        connect(load_, SIGNAL(clicked()), this, SLOT(load()));

        //Centrage du bouton Load
        QVBoxLayout* vLayout = new QVBoxLayout (); 
        vLayout->addWidget(view_);//on ajoute le view_ au vLayout

        QHBoxLayout* hLayout = new QHBoxLayout ();
        hLayout->addStretch ();  //on laisse un espace vide
        hLayout->addWidget ( load_ );//on postionne le bouton
        hLayout->addStretch ();
        vLayout->addLayout ( hLayout ); //on ajoute le hLayout au vLayout

        setLayout(vLayout); //on définit vLayout comme Layout principal
          }

    void  CellsLib::setCellViewer   ( CellViewer* cellviewer){
        cellViewer_ = cellviewer;
    }

    int   CellsLib::getSelectedRow  () const{
        QModelIndexList selecteds = view_->selectionModel()
                                        ->selection()
                                        .indexes();
        if (selecteds.empty())
            return -1;
        return selecteds.first().row();
    }

    void CellsLib ::load()
    {
        int selectedRow = getSelectedRow();
        if (selectedRow < 0)
            return;
        cellViewer_->setCell(baseModel_->getModel(selectedRow));
    }
}