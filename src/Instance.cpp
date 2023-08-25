

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "XmlUtil.h"
#include "Shape.h"
#include "Symbol.h"

namespace Netlist
{

    // Constructeur de l'instance
    /**
     * owner : cell propriétaire
     * model : cell esclave
     * name : nom de l'instance
     */
    Instance::Instance(Cell *owner, Cell *model, const std::string &name)
        : owner_(owner),
          masterCell_(model),
          name_(name),
          terms_(),
          position_()
    {
        std::vector<Term *> v = model->getTerms();
        for (std::vector<Term *>::const_iterator it = v.begin(); it != v.end(); ++it)
        {
            new Term(this, (*it));
        }
        owner_->add(this);
    }

    // Destructeur de l'instance
    Instance::~Instance()
    {
        for (std::vector<Term *>::iterator it = terms_.begin(); it != terms_.end(); ++it)
        {
            if ((*it) == NULL)
                continue;
            delete (*it);
        }
        // on retire l'instance de la cell
        owner_->remove(this);
    }

    const std::string &Instance::getName() const { return name_; }

    Cell *Instance::getCell() const { return owner_; }

    Cell *Instance::getMasterCell() const { return masterCell_; }

    const std::vector<Term *> &Instance::getTerms() const { return terms_; }

    // Accesseur à un term d'un instance
    /**
     * name : nom de l'instance
     * @return l'adresse du Term dont le nom est name
     */
    Term *Instance::getTerm(const std::string &name) const
    {
        for (std::vector<Term *>::const_iterator it = terms_.begin(); it != terms_.end(); ++it)
            if ((*it)->getName() == name)
                return (*it);
    }

    Point Instance::getPosition() const { return position_; }

    // Modificateur permettant de connecter un term à un net
    /**
     * name : nom du term
     * @return true si un term a été connceté, false sinon
     */
    bool Instance::connect(const std::string &name, Net *noeud)
    {
        Term *term = getTerm(name);
        if (term == NULL)
            return false;

        term->setNet(noeud);
        return true;
    }

    // ajout d'un term au vecteur terms_
    void Instance::add(Term *term)
    {
        terms_.push_back(term);
    }

    // retrait d'un term du vecteur terms_
    void Instance::remove(Term *term)
    {
        for (std::vector<Term *>::iterator iterm = terms_.begin(); iterm != terms_.end(); ++iterm)
        {
            if (((*iterm) == term) and (*iterm) != NULL)
                terms_.erase(iterm);
        }
    }

    void Instance::setPosition(const Point &p) { position_ = p; }

    void Instance::setPosition(int x, int y)
    {
        int deltaX;
        int deltaY;
        std::vector<Term *> terms = getMasterCell()->getTerms();
        for (std::vector<Term *>::const_iterator it = terms.begin(); it != terms.end(); ++it)
        {
            if (getMasterCell()->getSymbol()->getTermShape(*it) == NULL)
                continue;
            deltaX = getMasterCell()->getSymbol()->getTermShape(*it)->getX();
            deltaY = getMasterCell()->getSymbol()->getTermShape(*it)->getY();
            getTerm((*it)->getName())->setPosition(x + deltaX, y + deltaY); // translation du Term
        }
        position_ = Point(x,y);
    }

    void Instance::toXml(std::ostream &stream) const
    {
        stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << this->getMasterCell()->getName() << "\" x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>\n";
    }

    Instance *Instance::fromXml(Cell *c, xmlTextReaderPtr reader)
    {
        std::string instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"name")); // comparaison de pointeurs
        std::string masterCellName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"mastercell"));
        std::string xVal = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x"));
        std::string yVal = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y"));

        if (not(instanceName.empty() or masterCellName.empty() or xVal.empty() or yVal.empty()))
        {
            Instance *inst = new Instance(c, Cell::find(masterCellName), instanceName);
            inst->setPosition(atoi(xVal.c_str()), atoi(yVal.c_str()));
            return inst;
        }
        else
        {
            if (instanceName.empty())
            {
                std::cout << "ERROR sur instance" << std::endl;
            }

            if (masterCellName.empty())
            {
                std::cout << "ERROR sur mastercell" << std::endl;
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
}