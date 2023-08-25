#include "Shape.h"
#include "Symbol.h"
#include "XmlUtil.h"
#include "Cell.h"
#include "Term.h"

namespace Netlist
{
    using namespace std;

    // SHAPE
    Box Shape::getBoundingBox() const { return Box(); }

    Shape::Shape(Symbol *owner)
        : owner_(owner) { owner_->add(this); }

    Shape::~Shape() { owner_->remove(this); }

    Shape *Shape::fromXml(Symbol *owner, xmlTextReaderPtr reader)
    {
        // Factory-like method.
        const xmlChar *boxTag = xmlTextReaderConstString(reader, (const xmlChar *)"box");
        const xmlChar *ellipseTag = xmlTextReaderConstString(reader, (const xmlChar *)"ellipse");
        const xmlChar *arcTag = xmlTextReaderConstString(reader, (const xmlChar *)"arc");
        const xmlChar *lineTag = xmlTextReaderConstString(reader, (const xmlChar *)"line");
        const xmlChar *termTag = xmlTextReaderConstString(reader, (const xmlChar *)"term");
        const xmlChar *nodeName = xmlTextReaderConstLocalName(reader);

        Shape *shape = NULL;
        if (boxTag == nodeName)
            shape = BoxShape::fromXml(owner, reader);
        if (ellipseTag == nodeName)
            shape = EllipseShape::fromXml(owner, reader);
        if (arcTag == nodeName)
            shape = ArcShape::fromXml(owner, reader);
        if (lineTag == nodeName)
            shape = LineShape::fromXml(owner, reader);
        if (termTag == nodeName)
            shape = TermShape::fromXml(owner, reader);

        if (shape == NULL)
            cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
                 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

        return shape;
    }

    // BOXSHAPE
    BoxShape::BoxShape(Symbol *owner, const Box &box)
        : Shape(owner), box_(box) {}

    BoxShape::~BoxShape() {}

    Box BoxShape::getBoundingBox() const
    {
        return box_;
    }

    void BoxShape::toXml(std::ostream &stream) const
    {
        stream << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << " x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
    }

    BoxShape *BoxShape::fromXml(Symbol *owner, xmlTextReaderPtr reader)
    {
        std::string x1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x1")); // comparaison de pointeurs
        std::string y1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y1"));
        std::string x2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x2"));
        std::string y2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y2"));

        if (not(x1Val.empty() or y1Val.empty() or x2Val.empty() or y2Val.empty()))
        {
            int x1 = atoi(x1Val.c_str());
            int y1 = atoi(y1Val.c_str());
            int x2 = atoi(x2Val.c_str());
            int y2 = atoi(y2Val.c_str());

            BoxShape *boxshape = new BoxShape(owner, Box(x1, y1, x2, y2));
            return boxshape;
        }
        else
        {

            if (x1Val.empty())
            {
                std::cout << "ERROR sur x1Val" << std::endl;
            }

            if (y1Val.empty())
            {
                std::cout << "ERROR sur y1Val" << std::endl;
            }

            if (x2Val.empty())
            {
                std::cout << "ERROR sur x2Val" << std::endl;
            }

            if (y2Val.empty())
            {
                std::cout << "ERROR sur y2Val" << std::endl;
            }
            return NULL;
        }
    }

    // LINESHAPE
    LineShape ::LineShape(Symbol *owner, int x1, int y1, int x2, int y2)
        : Shape(owner), x1_(x1), y1_(y1), x2_(x2), y2_(y2) {}

    LineShape::~LineShape() {}

    Box LineShape::getBoundingBox() const
    {
        return Box(x1_, y1_, x2_, y2_);
    }

    void LineShape::toXml(std::ostream &stream) const
    {
        stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << " x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
    }

    LineShape *LineShape::fromXml(Symbol *owner, xmlTextReaderPtr reader)
    {
        std::string x1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x1")); // comparaison de pointeurs
        std::string y1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y1"));
        std::string x2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x2"));
        std::string y2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y2"));

        if (not(x1Val.empty() or y1Val.empty() or x2Val.empty() or y2Val.empty()))
        {
            int x1 = atoi(x1Val.c_str());
            int y1 = atoi(y1Val.c_str());
            int x2 = atoi(x2Val.c_str());
            int y2 = atoi(y2Val.c_str());

            LineShape *lineshape = new LineShape(owner, x1, y1, x2, y2);
            return lineshape;
        }
        else
        {

            if (x1Val.empty())
            {
                std::cout << "ERROR sur x1Val" << std::endl;
            }

            if (y1Val.empty())
            {
                std::cout << "ERROR sur y1Val" << std::endl;
            }

            if (x2Val.empty())
            {
                std::cout << "ERROR sur x2Val" << std::endl;
            }

            if (y2Val.empty())
            {
                std::cout << "ERROR sur y2Val" << std::endl;
            }
            return NULL;
        }
    }

    // TERMSHAPE
    TermShape ::TermShape(Symbol *owner, std::string name, int x, int y, NameAlign align)
        : Shape(owner), term_(NULL), x_(x), y_(y), align_(align)
    {
        term_ = owner->getCell()->getTerm(name);
    }

    TermShape ::~TermShape() {}

    Box TermShape ::getBoundingBox() const
    {
        return Box(x_, y_, x_+ 5, y_+5);
    }

    std::string TermShape::toString(NameAlign n)
    {
        if (n == 1)
        {
            return "top_left";
        }
        else if (n == 2)
        {
            return "top_right";
        }
        else if (n == 3)
        {
            return "bottom_left";
        }
        else if (n == 4)
        {
            return "bottom_right";
        }
        else
        {
            return "ERREUR l'align choisie est indéfinie";
        }
    }

    TermShape::NameAlign TermShape::toAlign(std::string s)
    {
        if (s == "top_left")
        {
            return top_left;
        }
        else if (s == "top_right")
        {
            return top_right;
        }
        else if (s == "bottom_left")
        {
            return bottom_left;
        }
        else if (s == "bottom_right")
        {
            return bottom_right;
        }
        else
        {
            std::cout << "ERREUR l'align choisie est indéfinie" << std::endl;
        }
    }

    void TermShape ::toXml(std::ostream &stream) const
    {
        stream << indent << "<term name=\"" << getTerm()->getName() << "\" x1=\"" << x_ << " y1=\"" << y_ << "\" align=\"" << toString(align_) << "\"/>\n";
    }

    TermShape *TermShape::fromXml(Symbol *owner, xmlTextReaderPtr reader)
    {
        std::string TermName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"name")); // comparaison de pointeurs
        std::string xVal = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x1"));
        std::string yVal = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y1"));
        std::string AlignName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"align"));

        if (not(TermName.empty() or xVal.empty() or yVal.empty() or AlignName.empty()))
        {
            int x = atoi(xVal.c_str());
            int y = atoi(yVal.c_str());

            TermShape *termshape = new TermShape(owner, TermName, x, y, toAlign(AlignName));
            return termshape;
        }
        else
        {

            if (TermName.empty())
            {
                std::cout << "ERROR sur TermName" << std::endl;
            }

            if (xVal.empty())
            {
                std::cout << "ERROR sur xVal" << std::endl;
            }

            if (yVal.empty())
            {
                std::cout << "ERROR sur yVal" << std::endl;
            }

            if (AlignName.empty())
            {
                std::cout << "ERROR sur AlignName" << std::endl;
            }
            return NULL;
        }
    }

    // ELLIPSESHAPE

    EllipseShape::EllipseShape(Symbol *owner, const Box &box)
        : Shape(owner), box_(box) {}

    EllipseShape::~EllipseShape() {}

    Box EllipseShape::getBoundingBox() const
    {
        return box_;
    }

    void EllipseShape::toXml(std::ostream &stream) const
    {
        stream << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << " x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
    }

    EllipseShape *EllipseShape::fromXml(Symbol *owner, xmlTextReaderPtr reader)
    {
        std::string x1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x1")); // comparaison de pointeurs
        std::string y1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y1"));
        std::string x2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x2"));
        std::string y2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y2"));

        if (not(x1Val.empty() or y1Val.empty() or x2Val.empty() or y2Val.empty()))
        {
            int x1 = atoi(x1Val.c_str());
            int y1 = atoi(y1Val.c_str());
            int x2 = atoi(x2Val.c_str());
            int y2 = atoi(y2Val.c_str());

            EllipseShape *ellipseshape = new EllipseShape(owner, Box(x1, y1, x2, y2));
            return ellipseshape;
        }
        else
        {

            if (x1Val.empty())
            {
                std::cout << "ERROR sur x1Val" << std::endl;
            }

            if (y1Val.empty())
            {
                std::cout << "ERROR sur y1Val" << std::endl;
            }

            if (x2Val.empty())
            {
                std::cout << "ERROR sur x2Val" << std::endl;
            }

            if (y2Val.empty())
            {
                std::cout << "ERROR sur y2Val" << std::endl;
            }
            return NULL;
        }
    }

    // ARCSHAPE

    ArcShape::ArcShape(Symbol *owner, const Box &box, int start, int span)
        : Shape(owner), box_(box), start_(start), span_(span) {}

    ArcShape::~ArcShape() {}

    Box ArcShape::getBoundingBox() const
    {
        return box_;
    }

    void ArcShape::toXml(std::ostream &stream) const
    {
        stream << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << " x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=" << start_ << "\" span=" << span_ << "\"/>\n";
    }

    ArcShape *ArcShape::fromXml(Symbol *owner, xmlTextReaderPtr reader)
    {
        std::string x1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x1")); // comparaison de pointeurs
        std::string y1Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y1"));
        std::string x2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"x2"));
        std::string y2Val = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"y2"));
        std::string startName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"start"));
        std::string spanName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar *)"span"));

        if (not(x1Val.empty() or y1Val.empty() or x2Val.empty() or y2Val.empty() or startName.empty() or spanName.empty()))
        {
            int x1 = atoi(x1Val.c_str());
            int y1 = atoi(y1Val.c_str());
            int x2 = atoi(x2Val.c_str());
            int y2 = atoi(y2Val.c_str());
            int start = atoi(startName.c_str());
            int span = atoi(spanName.c_str());

            ArcShape *arcshape = new ArcShape(owner, Box(x1, y1, x2, y2), start, span);
            return arcshape;
        }
        else
        {

            if (x1Val.empty())
            {
                std::cout << "ERROR sur x1Val" << std::endl;
            }

            if (y1Val.empty())
            {
                std::cout << "ERROR sur y1Val" << std::endl;
            }

            if (x2Val.empty())
            {
                std::cout << "ERROR sur x2Val" << std::endl;
            }

            if (y2Val.empty())
            {
                std::cout << "ERROR sur y2Val" << std::endl;
            }

            if (startName.empty())
            {
                std::cout << "ERROR sur startName" << std::endl;
            }

            if (spanName.empty())
            {
                std::cout << "ERROR sur spanName" << std::endl;
            }
            return NULL;
        }
    }
}
