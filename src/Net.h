#ifndef  NETLIST_NET_H   // Le define *doit* être différent dans
#define  NETLIST_NET_H   // chaque fichier (i.e. nom de classe).

#include  <libxml/xmlreader.h>
#include <string>

#include "Indentation.h"
#include "Term.h"

namespace Netlist {

  class Instance;
  class Cell;
  class Term;

  class Net {
    private:
        Cell*               owner_;
        std::string         name_;
        unsigned int        id_;
        Term::Type          type_;
        std::vector<Node*>  nodes_;
        std::vector<Line*>  lines_;
    public:   
                                            Net           ( Cell*, const std::string&, Term::Type );
                                            ~Net          ();
                    Cell*                   getCell       () const;
      const         std::string&            getName       () const;
      unsigned int                          getId         () const;
                    Term::Type              getType       () const;
                    std::vector<Node*>&     getNodes      () ;
                    size_t                  getFreeNodeId () const;
      void                                  add           ( Node* );
      bool                                  remove        ( Node* );
      void                                  toXml         ( std::ostream& stream ) const;
      static        Net*                    fromXml       (Cell*, xmlTextReaderPtr);

      inline const  std::vector<Line*>&     getLines      () const ;
      void                                  add           ( Line*);
      bool                                  remove        ( Line*);
                    Node*                   getNode       ( size_t);
  };

      inline const std::vector<Line*>&   Net::getLines () const { return lines_; }
}

#endif  // NETLIST_NET_H