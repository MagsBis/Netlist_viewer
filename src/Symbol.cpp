#include "Symbol.h"
#include "Shape.h"
#include "Cell.h"
#include "Term.h"
#include <vector>.h>

namespace Netlist
{

    using namespace std;

    Symbol::Symbol(Cell *cell)
        : owner_(cell), shapes_() {}

    Symbol::~Symbol()
    {
        for (std::vector<Shape *>::iterator it = shapes_.begin(); it != shapes_.end(); ++it)
        {
            if ((*it) == NULL)
                continue;
            delete (*it);
        }
    }

    Cell *Symbol::getCell() const { return owner_; }

    Box Symbol::getBoundingBox() const
    {
        Box bb;
        for (std::vector<Shape *>::const_iterator itshape = shapes_.begin(); itshape != shapes_.end(); ++itshape)
        {
            bb.merge((*itshape)->getBoundingBox());
        }
        return bb;
    }

    Point Symbol::getTermPosition(Term *term) const
    {
        if (term)
        {
            Point p = term->getPosition();
            return p;
        }
        else
        {
            cout << "Term Null" << endl;
        }
    }

    TermShape *Symbol::getTermShape(Term *term) const
    {
        for (std::vector<Shape *>::const_iterator itshape = shapes_.begin(); itshape != shapes_.end(); ++itshape)
        {
            TermShape *p = dynamic_cast<TermShape *>(*itshape);
            if (p)
            {
                if (p->getTerm()->getName() == term->getName())
                {
                    return p;
                }
            }
        }
        return NULL;
    }

    void Symbol::add(Shape *shape)
    {
        shapes_.push_back(shape);
    }

    void Symbol::remove(Shape *shape)
    {
        for (std::vector<Shape *>::iterator itshape = shapes_.begin(); itshape != shapes_.end(); ++itshape)
        {
            if (((*itshape) == shape) and (*itshape) != NULL)
                shapes_.erase(itshape);
        }
    }

    void Symbol::toXml(std::ostream &stream) const
    {
        stream << indent++ << "<symbol>\n";

        for (std::vector<Shape *>::const_iterator itshape = shapes_.begin(); itshape != shapes_.end(); ++itshape)
        {
            (*itshape)->toXml(stream);
        }

        stream << --indent << "</symbol>\n";
    }

    Symbol *Symbol::fromXml(Cell *cell, xmlTextReaderPtr reader)
    {

        const xmlChar *symbolTag = xmlTextReaderConstString(reader, (const xmlChar *)"symbol");
        Symbol *symbol = cell->getSymbol();

        while (true)
        {
            int status = xmlTextReaderRead(reader);
            if (status != 1)
            {
                if (status != 0)
                {
                    std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
                break;
            }

            switch (xmlTextReaderNodeType(reader))
            {
            case XML_READER_TYPE_COMMENT:
            case XML_READER_TYPE_WHITESPACE:
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar *nodeName = xmlTextReaderConstLocalName(reader);

            if ((nodeName == symbolTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))
            {
                break;
            }
            else
            {
                // cout << "test symbol" << endl;
                Shape::fromXml(symbol, reader);
            }
        }
        return symbol;
    }

    Symbol::Symbol(const Symbol &symbol)
        : owner_(symbol.owner_),
          shapes_()
    {
        std::vector<Shape *> shapes = symbol.getShapes(); // copie des shapes
        for (std::vector<Shape *>::const_iterator it = shapes.begin(); it != shapes.end(); ++it)
        {
            add(*it);
        }
    }

    Symbol &Symbol::operator=(const Symbol &symbol)
    {
        Symbol s = Symbol(symbol);
        return s;
    }

}
