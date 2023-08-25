// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>

class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


  class CellWidget : public QWidget {
      Q_OBJECT;
    public:
                            CellWidget          ( QWidget* parent=NULL );
      virtual               ~CellWidget         ();
            void            setCell             ( Cell* );
            inline  Cell*   getCell             () const;
            inline  int     xToScreenX          ( int x ) const;
            inline  int     yToScreenY          ( int y ) const;
            inline  QRect   boxToScreenRect     ( const Box& ) const;
            inline  QPoint  pointToScreenPoint  ( const Point& ) const;
            inline  int     screenXToX          ( int x ) const;
            inline  int     screenYToY          ( int y ) const;
            inline  Box     screenRectToBox     ( const QRect& ) const;
            inline  Point   screenPointToPoint  ( const QPoint& ) const;
            virtual QSize   minimumSizeHint     () const;
            virtual void    resizeEvent         ( QResizeEvent* );
            void            query               (unsigned int flags, QPainter &painter);
    protected:
            virtual void    paintEvent          ( QPaintEvent* );
            virtual void    keyPressEvent       ( QKeyEvent* );
    public slots:
            void            goLeft              ();
            void            goRight             ();
            void            goUp                ();
            void            goDown              ();
    private:
      Cell* cell_;
      Box   viewport_;
  };


  inline Cell* CellWidget::getCell () const { return cell_; }

            inline  int     CellWidget::xToScreenX         ( int x ) const{
                return (x - viewport_.getX1());
            }

            inline  int    CellWidget::yToScreenY         ( int y ) const{
                return (viewport_.getY2() - y);

            }
            inline  QRect   CellWidget::boxToScreenRect    ( const Box& b) const{ //QRect(x,y,largeur,hauteur)
                return QRect(xToScreenX(b.getX1()),yToScreenY(b.getY1()),xToScreenX(b.getX2())-xToScreenX(b.getX1()),yToScreenY(b.getY2())-yToScreenY(b.getY1()));
            }

            inline  QPoint  CellWidget::pointToScreenPoint ( const Point& p) const{
                return QPoint(xToScreenX(p.getX()),yToScreenY(p.getY()));
            }

            inline  int     CellWidget::screenXToX         ( int x ) const{
                return (x +viewport_.getX1());
            }

            inline  int     CellWidget::screenYToY         ( int y ) const{
                return (viewport_.getY2() + y);
            }

            inline  Box     CellWidget::screenRectToBox    ( const QRect& qR) const{
                return Box(screenXToX(qR.x()),screenYToY(qR.y()),screenXToX(qR.x())+qR.width(),screenYToY(qR.x())+qR.height());
            }

            inline  Point   CellWidget::screenPointToPoint ( const QPoint& qP) const{
                return Point(screenXToX(qP.x()),screenYToY(qP.y()));
            }


}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H