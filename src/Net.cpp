

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "XmlUtil.h"
#include "Line.h"

namespace Netlist
{

    using namespace std;

    // Constructuer du net
    /**
     * c : cell propriétaire
     * name : nom du net
     * t : type du net
     */
    Net::Net(Cell *c, const std::string &name, Term::Type t)
        : owner_(c),
          name_(name),
          id_(c->newNetId()),
          type_(t),
          nodes_()
    { // ajout du net à la cell
        c->add(this);
    }

    // Destructeur du net
    Net::~Net()
    {
        // mise à NULL des terms du net
        for (std::vector<Node *>::iterator it = nodes_.begin(); it != nodes_.end(); ++it)
        {
            if ((*it) == NULL)
                continue;
            NodeTerm *nt1 = dynamic_cast<NodeTerm *>(*it);
            nt1->getTerm()->setNet(NULL);
        }
        // retrait du net de son propriétaire
        owner_->remove(this);
    }

    // Accès à la cell propriétaire
    /**
     * n : noeud à retirer
     * @return la cell propriétaire du net
     */
    Cell *Net::getCell() const { return owner_; }

    const std::string &Net::getName() const { return name_; }

    unsigned int Net::getId() const { return id_; }

    Term::Type Net::getType() const { return type_; }

    std::vector<Node *> &Net::getNodes() { return nodes_; }

    // Méthode permettant d'obtenir un nouvel id pour un noeud
    /**
     * @return un nouvel id
     */
    size_t Net::getFreeNodeId() const
    {
        size_t id = 0;
        for (std::vector<Node *>::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it)
        {
            if ((*it) == NULL)
                return id; // si l'élément du vecteur pointe vers NULL alors la case est libre
            id++;
        }
        return nodes_.size();
    }

    // Méthode permettant d'ajouter un noeud au net
    /**
     * noeud : noeud à ajouter
     */
    void Net::add(Node *noeud)
    {

        if (not noeud)
            return;

        size_t id = noeud->getId();
        if (id == Node::noid)
        {
            id = getFreeNodeId(); // 3
            noeud->setId(id);
        }
        if (id < nodes_.size())
        {
            if (nodes_[id] != NULL)
            {
                std::cerr << "Overwrite" << id << std::endl;
                nodes_[id]->setId(Node::noid);
            }
            nodes_[id] = noeud;
        }
        else
        {
            for (size_t i = nodes_.size(); i < id; ++i)
                nodes_.push_back(NULL);
            nodes_.push_back(noeud);
        }
    }

    // retrait d'un noeud du net
    /**
     * n : noeud à retirer
     * @return true si un noeud a pu être ajouté, false sinon
     */
    bool Net::remove(Node *n)
    {
        if (n == NULL)
        {
            return false;
        }
        else
        {
            size_t id = n->getId();
            nodes_.insert(nodes_.begin() + id, NULL); // on fait pointer la case vers null lorsqu'on retire un term
        }
        return true;
    }

    void Net::toXml(std::ostream &stream) const
    {
        stream << indent++ << "<net name=\"" << name_ << "\" type=\"" << Term::toString(this->getType()) << "\">\n";
        // affichage des nodes du net
        for (std::vector<Node *>::const_iterator itnode = nodes_.begin(); itnode != nodes_.end(); ++itnode)
        {
            if ((*itnode) == NULL)
                continue;
            (*itnode)->toXml(stream);
        }

        for (std::vector<Line *>::const_iterator itline = lines_.begin(); itline != lines_.end(); ++itline)
        {
            if ((*itline) == NULL)
                continue;
            (*itline)->toXml(stream);
        }

        stream << --indent << "</net>\n";
    }

    Net *Net::fromXml(Cell *c, xmlTextReaderPtr reader)
    {

        std::string netName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"name")); // comparaison de pointeurs
        std::string typeTag = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"type"));

        if (not(netName.empty() or typeTag.empty()))
        {
            Term::Type type = Term::toType(typeTag);
            Net *net = new Net(c, netName, type);
            net->id_ = c->newNetId();

            const xmlChar *nodeTag = xmlTextReaderConstString(reader, (const xmlChar *)"node");
            const xmlChar *lineTag = xmlTextReaderConstString(reader, (const xmlChar *)"line");

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

                if (nodeTag == nodeName)
                {
                    // cout << "test term" << endl;
                    if (Node::fromXml(net, reader))
                        continue;
                }

                if (lineTag == nodeName)
                {
                    // cout << "test line" << endl;
                    if (Line::fromXml(net, reader))
                        continue;
                }
                return net;
            }
        }
        else
        {
            if (netName.empty())
            {
                std::cout << "ERROR sur netName" << std::endl;
            }

            if (typeTag.empty())
            {
                std::cout << "ERROR sur typeTag" << std::endl;
            }
            return NULL;
        }
    }

    void Net::add(Line *line)
    {
        if (line)
            lines_.push_back(line);
    }

    bool Net::remove(Line *line)
    {
        if (line)
        {
            for (vector<Line *>::iterator il = lines_.begin(); il != lines_.end(); ++il)
            {
                if (*il == line)
                {
                    lines_.erase(il);
                    return true;
                }
            }
        }
        return false;
    }

    Node *Net::getNode(size_t id)
    {
        for (std::vector<Node *>::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it)
        {
            if ((*it) == NULL)
                continue;
            if ((*it)->getId() == id)
                return (*it);
        }
    }
} // Netlist namespace