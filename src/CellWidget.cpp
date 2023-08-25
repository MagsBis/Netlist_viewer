// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QApplication>
#include "CellWidget.h"
#include "Term.h"
#include "Instance.h"
#include "Symbol.h"
#include "Shape.h"
#include "Cell.h"
#include "Line.h"
#include "Node.h"
#include "Net.h"
#include "Shape.h"


#define NAVY QColor(38, 73, 92, 255) 

#define LIGHT_GREY QColor(229, 229, 220, 255)

#define ORANGE QColor(210, 96, 26, 255)

namespace Netlist
{
    unsigned int LineShapes = 1;
    unsigned int InstanceShapes = 2;
    unsigned int TermShapes = 3;

    using namespace std;

    ostream &operator<<(ostream &o, const QRect &rect)
    {
        o << "<QRect x:" << rect.x()
          << " y:" << rect.y()
          << " w:" << rect.width()
          << " h:" << rect.height() << ">";
        return o;
    }

    ostream &operator<<(ostream &o, const QPoint &p)
    {
        o << "<QRect x:" << p.x() << " y:" << p.y() << ">";
        return o;
    }

    CellWidget::CellWidget(QWidget *parent)
        : QWidget(parent), cell_(NULL), viewport_(Box(0, 0, 500, 500))
    {
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_StaticContents);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
    }

    CellWidget::~CellWidget()
    {
    }

    void CellWidget::setCell(Cell *cell)
    {
        cell_ = cell;
        repaint();
    }

    void CellWidget::goLeft()
    {
        viewport_.translate(Point(-20, 0));
        repaint();
    }

    void CellWidget::goRight()
    {
        viewport_.translate(Point(20, 0));
        repaint();
    }

    void CellWidget::goUp()
    {
        viewport_.translate(Point(0, 20));
        repaint();
    }

    void CellWidget::goDown()
    {
        viewport_.translate(Point(0, -20));
        repaint();
    }

    QSize CellWidget::minimumSizeHint() const
    {
        return QSize(500, 500);
    }

    void CellWidget::resizeEvent(QResizeEvent *event)
    {
        const QSize &size = event->size();
        // Assume the resize is always done by drawing the bottom right corner .
        viewport_.setX2(viewport_.getX1() + size.width());
        viewport_.setY1(viewport_.getY2() - size.height());
        cerr << " CellWidget :: resizeEvent () viewport_ : " << viewport_ << endl;
        repaint();
    }

    void CellWidget::paintEvent(QPaintEvent *event)
    {
        QFont bigFont = QFont("URW Bookman L", 36);

        QString cellName = "NULL";
        if (cell_)
            cellName = cell_->getName().c_str();

        QPainter painter(this);
        painter.setFont(bigFont);
        painter.setBackground(QBrush(Qt::black));
        painter.eraseRect(QRect(QPoint(0, 0), size()));

        int frameWidth = 460;
        int frameHeight = 100;
        QRect nameRect((size().width() - frameWidth) / 2, (size().height() - frameHeight) / 2, frameWidth, frameHeight);

        painter.drawRect(nameRect);
        painter.drawText(nameRect, Qt::AlignCenter, cellName);

        QFont bigFont2 = QFont("URW Bookman L", 8);

        painter.setFont(bigFont2);

        query(LineShapes, painter);
        query(InstanceShapes, painter);
        query(TermShapes, painter);
    }

    void CellWidget::keyPressEvent(QKeyEvent *event)
    {
        event->ignore();
        if (event->modifiers() & (Qt ::ControlModifier | Qt ::ShiftModifier))
            return;
        switch (event->key())
        {
        case Qt ::Key_Up:
            goUp();
            break;
        case Qt ::Key_Down:
            goDown();
            break;
        case Qt ::Key_Left:
            goLeft();
            break;
        case Qt ::Key_Right:
            goRight();
            break;
        default:
            return;
        }
        event->accept();
    }

    void CellWidget ::query(unsigned int flags, QPainter &painter)
    {
        if ((not cell_) or (not flags))
            return;

        if (flags & LineShapes)
        {
            const vector<Net *> &nets = cell_->getNets();
            for (size_t i = 0; i < nets.size(); ++i)
            {
                const vector<Line *> &lines = nets[i]->getLines();
                for (size_t j = 0; j < lines.size(); ++j)
                { // affichage des lines du net
                    painter.setPen(QPen(LIGHT_GREY, 1));
                    painter.setBrush(QBrush(LIGHT_GREY));
                    painter.drawLine(pointToScreenPoint(lines[j]->getSourcePosition()),
                                     pointToScreenPoint(lines[j]->getTargetPosition()));
                }
                const vector<Node *> &nodes = nets[i]->getNodes();
                for (size_t j = 0; j < nodes.size(); ++j)
                { // affichage des nodes du net
                    NodePoint *node = dynamic_cast<NodePoint *>(nodes[j]);
                    if (node == NULL)
                        continue;
                    if (node->getLines().size() > 2)
                    { // creation d'un equipotentiel en cas d'intersection de fils
                        painter.setPen(QPen(LIGHT_GREY, 1));
                        painter.setBrush(QBrush(LIGHT_GREY));
                        painter.drawEllipse(pointToScreenPoint(node->getPosition()), 4, 4); //
                    }
                }
            }
        }

        const vector<Instance *> &instances = cell_->getInstances();
        if (instances.size() > 0)
        { // cas ou on a des instances
            for (size_t i = 0; i < instances.size(); ++i)
            {
                Point instPos = instances[i]->getPosition();
                const Symbol *symbol = instances[i]->getMasterCell()->getSymbol();
                if (not symbol)
                    continue;
                if (flags & InstanceShapes)
                {
                    const vector<Shape *> &shapes = symbol->getShapes();
                    for (size_t j = 0; j < shapes.size(); ++j)
                    {

                        BoxShape *boxShape = dynamic_cast<BoxShape *>(shapes[j]);
                        if (boxShape)
                        {
                            Box box = boxShape->getBoundingBox();
                            QRect rect = boxToScreenRect(box.translate(instPos));
                            painter.setPen(QPen(NAVY, 3));
                            painter.setBrush(QBrush(Qt ::black));
                            painter.drawRect(rect);
                        }
                        painter.setPen(QPen(NAVY, 1));
                        painter.setBrush(QBrush(NAVY));
                        QRect rect2 = QRect(xToScreenX(instPos.getX()) - 20, yToScreenY(instPos.getY()) - 5, 100, 30);
                        painter.drawText(rect2, Qt::AlignCenter | Qt::AlignBottom, instances[i]->getName().c_str());

                        LineShape *lineShape = dynamic_cast<LineShape *>(shapes[j]);
                        if (lineShape)
                        {
                            Point p1 = Point(lineShape->getX1(), lineShape->getY1());
                            Point p2 = Point(lineShape->getX2(), lineShape->getY2());
                            p1 = p1.translate(instPos);
                            p2 = p2.translate(instPos);
                            painter.setPen(QPen(NAVY, 3));
                            painter.setBrush(QBrush(NAVY));
                            painter.drawLine(xToScreenX(p1.getX()),
                                             yToScreenY(p1.getY()),
                                             xToScreenX(p2.getX()),
                                             yToScreenY(p2.getY()));
                        }

                        EllipseShape *ellipseShape = dynamic_cast<EllipseShape *>(shapes[j]);
                        if (ellipseShape)
                        {//affichage des cercles, ex: transistorP
                            Box box = ellipseShape->getBoundingBox();
                            QRect rect = boxToScreenRect(box.translate(instPos));
                            painter.setPen(QPen(NAVY, 3));
                            painter.setBrush(QBrush(Qt ::black));
                            painter.drawEllipse(rect);
                        }

                        ArcShape *arcShape = dynamic_cast<ArcShape *>(shapes[j]);
                        if (arcShape)
                        {//affichage des courbures des instances
                            Box box = arcShape->getBoundingBox();
                            QRect rect = boxToScreenRect(box.translate(instPos));
                            painter.setPen(QPen(NAVY, 3));
                            painter.setBrush(QBrush(NAVY));
                            painter.drawArc(rect,
                                            arcShape->getStart() * 16,
                                            arcShape->getSpan() * 16);
                        }
                    }
                }
            }
        }

        // TERMSHAPES AVEC INSTANCES
        if (instances.size() > 0)
        { // cas ou on a des instances
            for (size_t i = 0; i < instances.size(); ++i)
            {
                Point instPos = instances[i]->getPosition();
                const Symbol *symbol = instances[i]->getMasterCell()->getSymbol();
                if (not symbol)
                    continue;
                if (flags & TermShapes)
                {
                    const vector<Shape *> &shapes = symbol->getShapes();
                    for (size_t j = 0; j < shapes.size(); ++j)
                    {

                        TermShape *termShape = dynamic_cast<TermShape *>(shapes[j]);
                        if (termShape)
                        {
                            Box box = termShape->getBoundingBox();
                            QRect rect = boxToScreenRect(box.translate(instPos));
                            painter.setPen(QPen(ORANGE, 1));
                            painter.setBrush(QBrush(ORANGE));
                            painter.drawRect(QRect(rect.x() - 2, rect.y() + 2, rect.width(), rect.height()));

                            QRect rect2;

                            painter.setPen(QPen(ORANGE, 1));
                            if (termShape->getAlign() == TermShape::top_left)
                            {
                                rect2 = QRect(rect.x() - 10, rect.y() - 20, 20, 20);
                                painter.drawText(rect2, Qt::AlignLeft | Qt::AlignTop, termShape->getTerm()->getName().c_str());
                            }

                            else if (termShape->getAlign() == TermShape::top_right)
                            {
                                if (strlen(termShape->getTerm()->getName().c_str()) > 1)
                                { // nom de term grand
                                    rect2 = QRect(rect.x() - 15, rect.y() - 15, 40, 20);
                                    painter.drawText(rect2, Qt::AlignRight | Qt::AlignTop, termShape->getTerm()->getName().c_str());
                                }
                                if (strlen(termShape->getTerm()->getName().c_str()) == 1)
                                { // nom de term petit
                                    rect2 = QRect(rect.x() - 15, rect.y() - 15, 35, 20);
                                    painter.drawText(rect2, Qt::AlignRight | Qt::AlignTop, termShape->getTerm()->getName().c_str());
                                }
                            }

                            else if (termShape->getAlign() == TermShape::bottom_left)
                            {
                                rect2 = QRect(rect.x() - 15, rect.y() - 20, 20, 20);
                                painter.drawText(rect2, Qt::AlignLeft | Qt::AlignBottom, termShape->getTerm()->getName().c_str());
                            }

                            else if (termShape->getAlign() == TermShape::bottom_right)
                            {
                                if (strlen(termShape->getTerm()->getName().c_str()) > 1)
                                { // nom de term grand
                                    rect2 = QRect(rect.x() - 25, rect.y() - 20, 50, 40);
                                    painter.drawText(rect2, Qt::AlignRight | Qt::AlignBottom, termShape->getTerm()->getName().c_str());
                                }
                                if (strlen(termShape->getTerm()->getName().c_str()) == 1)
                                { // nom de term petit
                                    rect2 = QRect(rect.x() - 25, rect.y() - 20, 45, 40);
                                    painter.drawText(rect2, Qt::AlignRight | Qt::AlignBottom, termShape->getTerm()->getName().c_str());
                                }
                            }
                            else
                            {
                                std::cout << "alignement indefinie" << std::endl;
                            }
                        }
                    }

                    // connecteurs
                    const vector<Term *> &terms = cell_->getTerms();
                    for (size_t i = 0; i < terms.size(); ++i)
                    {
                        Point p = terms[i]->getPosition();

                        Box box = Box(p.getX(),
                                      p.getY(),
                                      p.getX(),
                                      p.getY());

                        QRect rect = boxToScreenRect(box);

                        QPointF qp = pointToScreenPoint(p);
                        qp = QPointF(qp.x() - 7, qp.y() - 7);

                        QPointF points[5] = {
                            QPointF(qp.x(), qp.y()),
                            QPointF(qp.x() + 7, qp.y()),
                            QPointF(qp.x() + 14, qp.y() + 7),
                            QPointF(qp.x() + 7, qp.y() + 14),
                            QPointF(qp.x(), qp.y() + 14)};

                        painter.setPen(QPen(ORANGE, 1));
                        painter.setBrush(QBrush(ORANGE));
                        painter.drawPolygon(points, 5);

                        QRect rect2;
                        painter.setPen(QPen(ORANGE, 1));

                        if (terms[i]->getDirection() == Term::In)
                        {
                            rect2 = QRect(rect.x() - 25, rect.y() - 20, 20, 30);
                            painter.drawText(rect2, Qt::AlignLeft | Qt::AlignBottom, terms[i]->getName().c_str());
                        }
                        else if (terms[i]->getDirection() == Term::Out)
                        {
                            rect2 = QRect(rect.x() - 20, rect.y() - 20, 55, 40);
                            painter.drawText(rect2, Qt::AlignRight | Qt::AlignBottom, terms[i]->getName().c_str());
                        }
                        else
                        {
                            std::cout << "direction indefinie" << std::endl;
                        }
                    }
                }
            }
        }

        if (instances.size() == 0)
        {                        // cas ou on n'a pas d'instance
            Point pos = Point(250, 250); 
            const Symbol *symbol = cell_->getSymbol();
            if (not symbol)
                return;
            if (flags & InstanceShapes)
            {
                const vector<Shape *> &shapes = symbol->getShapes();
                for (size_t j = 0; j < shapes.size(); ++j)
                {
                    BoxShape *boxShape = dynamic_cast<BoxShape *>(shapes[j]);
                    if (boxShape)
                    {
                        Box box = boxShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(pos));
                        painter.setPen(QPen(NAVY, 1));
                        painter.setBrush(QBrush(NAVY));
                        painter.drawRect(rect);

                        QRect rect2 = QRect(rect.x(), rect.y(), rect.x() + 50, rect.y() + 50);
                        painter.drawText(rect2, Qt::AlignCenter | Qt::AlignBottom, symbol->getCell()->getName().c_str());
                    }

                    painter.setPen(QPen(NAVY, 1));
                    painter.setBrush(QBrush(NAVY));
                    QRect rect2 = QRect(xToScreenX(pos.getX()) - 20, yToScreenY(pos.getY()) - 5, 100, 30);
                    painter.drawText(rect2, Qt::AlignCenter | Qt::AlignBottom, cell_->getName().c_str());

                    LineShape *lineShape = dynamic_cast<LineShape *>(shapes[j]);
                    if (lineShape)
                    {

                        Point p1 = Point(Point(lineShape->getX1(), lineShape->getY1()).translate(pos));
                        Point p2 = Point(Point(lineShape->getX2(), lineShape->getY2()).translate(pos));
                        painter.setPen(QPen(NAVY, 1));
                        painter.setBrush(QBrush(NAVY));
                        painter.drawLine(xToScreenX(p1.getX()),
                                         yToScreenY(p1.getY()),
                                         xToScreenX(p2.getX()),
                                         yToScreenY(p2.getY()));
                    }

                    EllipseShape *ellipseShape = dynamic_cast<EllipseShape *>(shapes[j]);
                    if (ellipseShape)
                    {
                        Box box = ellipseShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(pos));
                        painter.setPen(QPen(NAVY, 1));
                        painter.setBrush(QBrush(Qt ::black));
                        painter.drawEllipse(rect);
                    }

                    ArcShape *arcShape = dynamic_cast<ArcShape *>(shapes[j]);
                    if (arcShape)
                    {
                        Box box = arcShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(pos));
                        painter.setPen(QPen(NAVY, 1));
                        painter.setBrush(QBrush(NAVY));
                        painter.drawArc(rect,
                                        arcShape->getStart() * 16,
                                        arcShape->getSpan() * 16);
                    }
                }
            }
        }
        // TERMSHAPES SANS INSTANCES
        if (instances.size() == 0)
        {                                // cas ou on n'a pas d'instance
            Point pos = Point(250, 250); 
            const Symbol *symbol = cell_->getSymbol();
            if (not symbol)
                return;
            if (flags & TermShapes)
            {
                const vector<Shape *> &shapes = symbol->getShapes();
                for (size_t j = 0; j < shapes.size(); ++j)
                {
                    TermShape *termShape = dynamic_cast<TermShape *>(shapes[j]);
                    if (termShape)
                    {
                        Box box = termShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(pos));
                        painter.setPen(QPen(ORANGE, 1));
                        painter.setBrush(QBrush(ORANGE));
                        painter.drawRect(QRect(rect.x() - 2, rect.y() + 2, rect.width(), rect.height()));

                        QRect rect2;

                        rect2 = QRect(rect.x() - 22, rect.y() - 22, 20, 28);
                        painter.drawText(rect2, Qt::AlignLeft | Qt::AlignBottom, termShape->getTerm()->getName().c_str());
                    }
                }
            }
        }
    }

} // Netlist namespace.
