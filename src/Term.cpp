

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "XmlUtil.h"

namespace Netlist
{
    // Constructeur de Term si le propriétaire est une cell
    Term::Term(Cell *cell, const std::string &name, Direction direction)
        : owner_(cell),
          name_(name),
          direction_(direction),
          type_(External),
          net_(NULL),
          node_(this, Node::noid)
    {
        cell->add(this);
    }

    // Constructeur de Term si le propirétaire est une instance
    Term::Term(Instance *inst, Term *modelTerm)
        : owner_(inst),
          name_(modelTerm->name_),
          direction_(modelTerm->direction_),
          type_(Internal),
          net_(NULL),
          node_(this, Node::noid)
    {
        inst->add(this);
    }

    Term::~Term()
    {
        // retrait du terminal de son propriétaire
        if (isExternal())
            getCell()->remove(this);

        // retrait du terminal du net
        if (net_ != NULL)
        {
            setNet(NULL);
        }
    }

    std::string Term::toString(Type t)
    {
        if (t == Internal)
            return "Internal";
        return "External";
    }

    std::string Term::toString(Direction d)
    {
        if (d == 1)
        {
            return "In";
        }
        else if (d == 2)
        {
            return "Out";
        }
        else if (d == 3)
        {
            return "Inout";
        }
        else if (d == 4)
        {
            return "Tristate";
        }
        else if (d == 5)
        {
            return "Transcv";
        }
        else if (d == 6)
        {
            return "Unknown";
        }
        else
        {
            return "ERREUR le type choisie est indéfinie";
        }
    }

    Term::Direction Term::toDirection(std::string s)
    {
        if (s == "In")
        {
            return In;
        }
        else if (s == "Out")
        {
            return Out;
        }
        else if (s == "Inout")
        {
            return Inout;
        }
        else if (s == "Tristate")
        {
            return Tristate;
        }
        else if (s == "Transcv")
        {
            return Transcv;
        }
        else if (s == "Unknown")
        {
            return Unknown;
        }
        else
        {
            std::cout << "ERREUR la direction choisie est indéfinie" << std::endl;
        }
    }

    Term::Type Term::toType(std::string s)
    {
        if (s == "Internal")
        {
            return Internal;
        }
        else if (s == "External")
        {
            return External;
        }
        else
        {
            std::cout << "ERREUR le type choisie est indéfinie" << std::endl;
        }
    }

    // ajout d'un net au noeud
    /**
     * n : net à ajouter à term et où on insert le noeud ou un NULL
     */
    void Term::setNet(Net *n)
    {
        // ajout du noeud au net
        if (n != NULL)
        {
            net_ = n;
            net_->add(&node_);
        }
        else
        { // insertion d'un NULL dans net à la place du noeud
            std::vector<Node *> nodes = net_->getNodes();
            nodes.insert(nodes.begin() + node_.getId(), NULL);
            net_ = NULL;
        }
    }

    // surcharge de setNet
    /**
     * name : nom du net auquel on ajoute le noeud
     */
    void Term::setNet(const std::string &name)
    {
        setNet(getOwnerCell()->getNet(name));
    }

    void Term::setPosition(const Point &p) { node_.setPosition(p); }

    void Term::setPosition(int x, int y) { node_.setPosition(x, y); }

    // Accès à la cell propriétaire
    /**
     * @return la cell propriétaire du term si ce dernier est External, la cell propriétaire de l'instance sinon (si le term est Internal)
     */
    Cell *Term::getOwnerCell() const
    {
        if (isExternal())
        {
            return static_cast<Cell *>(owner_);
        }
        else if (isInternal())
        {
            return static_cast<Instance *>(owner_)->getCell();
        }
    }

    void Term::toXml(std::ostream &stream) const
    {
        stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_) << " x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>\n";
    }

    Term *Term::fromXml(Cell *c, xmlTextReaderPtr reader)
    {
        std::string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"name")); // comparaison de pointeurs
        std::string directionType = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"direction"));
        std::string xVal = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x"));
        std::string yVal = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y"));

        if (not(termName.empty() or directionType.empty() or xVal.empty() or yVal.empty()))
        {
            Direction d = toDirection(directionType);
            Term *term = new Term(c, termName, d);
            term->setPosition(atoi(xVal.c_str()), atoi(yVal.c_str()));
            return term;
        }
        else
        {

            if (termName.empty())
            {
                std::cout << "ERROR sur termName" << std::endl;
            }

            if (directionType.empty())
            {
                std::cout << "ERROR sur directionType" << std::endl;
            }

            if (xVal.empty())
            {
                std::cout << "ERROR sur xVal" << std::endl;
            }

            if (yVal.empty())
            {
                std::cout << "ERROR sur yVal" << std::endl;
            }
            return NULL;
        }
    }
} // Netlist namespace