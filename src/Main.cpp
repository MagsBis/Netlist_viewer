#include <QApplication>
#include <QtGui>

#include "CellViewer.h"
#include "Instance.h"

using namespace Netlist;
using namespace std;

int main(int argc, char *argv[])
{
    cout << "Chargement des modeles..." << endl;
    Cell::load("vdd");
    Cell::load("gnd");
    Cell::load("TransistorN");
    Cell::load("TransistorP");
    Cell::load("and2");
    Cell::load("or2");
    Cell *xor2 = Cell::load("xor2");
    Cell *halfadder = Cell::load("halfadder");
    Cell *fulladder = Cell::load("fulladder");

    QApplication* qa = new QApplication(argc, argv);
    CellViewer* viewer = new CellViewer();
    viewer->setCell(halfadder);
    viewer->show();
    int rvalue = qa->exec();
    delete qa;
    return rvalue;
}