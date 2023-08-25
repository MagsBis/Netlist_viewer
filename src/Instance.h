#ifndef  NETLIST_INSTANCE_H   
#define  NETLIST_INSTANCE_H  

#include  <libxml/xmlreader.h>
#include <iostream>
#include <vector>
#include "Point.h"
#include "Cell.h"

namespace Netlist {

    class Term;
    class Net;
    class Cell;

    class Instance{
    private :
        Cell*               owner_;
        Cell*               masterCell_;
        std::string         name_;
        std::vector<Term*>  terms_;
        Point               position_;
        
    public :
                                    Instance      ( Cell* owner, Cell* model, const std::string& );
                                    ~Instance      ();
        const   std::string&        getName       () const;
                Cell*               getMasterCell () const;
                Cell*               getCell       () const;
        const   std::vector<Term*>& getTerms      () const;
                Term*               getTerm       ( const std::string& ) const;
                Point               getPosition   () const;
        bool                        connect       ( const std::string& name, Net* );
        void                        add           ( Term* );
        void                        remove        ( Term* );
        void                        setPosition   ( const Point& );
        void                        setPosition   ( int x, int y );
        void                        toXml         ( std::ostream& stream ) const;
        static Instance*            fromXml       (Cell*, xmlTextReaderPtr);

    };
}

#endif  // NETLIST_INSTANCE_H
