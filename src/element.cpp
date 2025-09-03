#include "element.h"
#include <cstdint>
#include <fstream>

namespace ds_imp {

/* Constructors */
Element::Element() : Element::Element(DEFAULT_ELEMENT) {}
Element::Element(int32_t value) : val(value) {}

/* Destructor */
Element::~Element() = default;

/* Copy constructor & Assignment */
Element::Element(const Element &other) : val(other.val) {}
Element& Element::operator=(const Element &other) { val = other.val; return *this; }

/* Move constructor & Move assignment */
Element::Element(Element &&other) noexcept : val(other.val) {}
Element& Element::operator=(Element &&other) noexcept { 
    
    if (this == &other) return *this;
    val = other.val;    return *this; 
}

std::ostream& operator<<(std::ostream& os, const Element& obj) {
    os << obj.val;
    return os;
}

std::istream& operator>>(std::istream& is, Element& obj) {
    if(!(is >> obj.val)) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

/* Getter & Setter */
int32_t Element::get() const {
    return this->val;
}

void Element::set(int32_t value) {
    this->val = value;
}

}