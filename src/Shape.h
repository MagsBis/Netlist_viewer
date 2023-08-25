// -*- explicit-buffer-name: "Box.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include  <libxml/xmlreader.h>
#include "Box.h"

namespace Netlist {
    class Symbol;
    class Term;

    class Line;
    class Cell;


    class Shape{
        public : 
                                        Shape (Symbol*);
            virtual                     ~Shape ();
            virtual         Box         getBoundingBox() const = 0;
            inline          Symbol*     getSymbol() const;
            virtual void                toXml ( std::ostream& ) const = 0;
            static          Shape*      fromXml ( Symbol* owner, xmlTextReaderPtr reader );
        private :
            Symbol* owner_;
    };

    inline Symbol* Shape :: getSymbol () const { return owner_ ; }

    class BoxShape : public Shape{
        public :
                                        BoxShape (Symbol*, const Box&);
                                        ~BoxShape ();
            virtual         Box         getBoundingBox() const;
            virtual void                toXml ( std::ostream& ) const;
            static          BoxShape*   fromXml( Symbol* owner, xmlTextReaderPtr reader );
        private : 
            Box box_; 
    };

    class LineShape : public Shape{
        public :
                                        LineShape (Symbol*, int x1, int y1, int x2, int y2);
                                        ~LineShape ();
            virtual         Box         getBoundingBox() const;
            virtual void                toXml ( std::ostream& stream ) const;
            static          LineShape*  fromXml( Symbol* owner, xmlTextReaderPtr reader );
            inline int                  getX1() const;
            inline int                  getY1() const;
            inline int                  getX2() const;
            inline int                  getY2() const;
        private : 
            Symbol* owner_; 
            int x1_,y1_,x2_,y2_;
    };

            inline int LineShape::getX1() const{return x1_;}
            inline int LineShape::getY1() const{return y1_;}
            inline int LineShape::getX2() const{return x2_;}
            inline int LineShape::getY2() const{return y2_;}

    class TermShape : public Shape{
        public : 
            enum NameAlign { top_left=1, top_right, bottom_left, bottom_right };
        public :
                                         TermShape      (Symbol*,std::string, int x, int y, NameAlign);
                                        ~TermShape      ();
            virtual         Box          getBoundingBox () const;
            inline          Term*        getTerm       () const;
            inline int                   getX           () const;
            inline int                   getY           () const;
            virtual void                 toXml          ( std::ostream& stream ) const;
            static          std::string  toString       ( NameAlign );
            static          NameAlign    toAlign        ( std::string );
            static          TermShape*   fromXml( Symbol* owner, xmlTextReaderPtr reader );
            inline          NameAlign    getAlign       () const;
        private : 
            Term*       term_;
            int         x_,y_;
            NameAlign   align_;
    };

    inline     Term* TermShape::getTerm() const{ return term_;}
    inline int TermShape::getX() const{return x_;}
    inline int TermShape::getY() const{return y_;}
    inline     TermShape::NameAlign TermShape::getAlign () const{return align_;}


    class EllipseShape : public Shape{
        public :
                                            EllipseShape      (Symbol* , const Box & );
                                            ~EllipseShape     ();
            virtual         Box             getBoundingBox    () const;
            virtual void                    toXml             ( std::ostream& ) const;
            static          EllipseShape*   fromXml( Symbol* owner, xmlTextReaderPtr reader );
        private : 
            Box     box_;

    };

    class ArcShape : public Shape{
        public :
                                             ArcShape       (Symbol* , const Box &, int, int);
                                            ~ArcShape       ();
            virtual         Box             getBoundingBox () const;
            inline int                      getStart           () const;
            inline int                      getSpan           () const;
            virtual void                    toXml          ( std::ostream& ) const;
            static          ArcShape*       fromXml( Symbol* owner, xmlTextReaderPtr reader );

        private : 
            Box     box_;
            int start_,span_;

    };

            inline int ArcShape::getStart () const{ return start_;}
            inline int ArcShape::getSpan () const{return span_;}

} // Netlist namespace.
#endif  // NETLIST_SHAPE_H