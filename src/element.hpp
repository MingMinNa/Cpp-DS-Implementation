#pragma once

#include <cstdint>
#include <fstream>

namespace ds_imp {

class Element {

    public:
        /* Constructors */
        Element();
        Element(int32_t value);

        /* Destructor */
        ~Element();

        /* Copy constructor & Assignment */
        Element(const Element &other);
        Element& operator=(const Element &other);

        /* Move Constructor & Move Assignment */
        Element(Element &&other) noexcept;            
        Element& operator=(Element &&other) noexcept;

        /* Other operators */
        // https://en.cppreference.com/w/cpp/language/operators.html

        /* Comparison operators */
        inline bool operator==(const Element &other) const { return val == other.val; }
        inline bool operator!=(const Element &other) const { return val != other.val; }
        inline bool operator< (const Element &other) const { return val < other.val;  }
        inline bool operator<=(const Element &other) const { return val <= other.val; }
        inline bool operator> (const Element &other) const { return val > other.val;  }
        inline bool operator>=(const Element &other) const { return val >= other.val; }

        /* Arithmetic operators */
        inline Element operator+(const Element &other) const { return Element(this->val + other.val); }
        inline Element operator-(const Element &other) const { return Element(this->val - other.val); }
        inline Element operator*(const Element &other) const { return Element(this->val * other.val); }
        inline Element operator/(const Element &other) const { return Element(this->val / other.val); }

        friend std::ostream& operator<<(std::ostream& os, const Element& obj);
        friend std::istream& operator>>(std::istream& is, Element& obj);

        /* Getter & Setter */
        int32_t get() const;
        void set(int32_t value);

    private:
        int32_t val;
};

inline const Element UNIT_ELEMENT(1);
inline const Element MAX_ELEMENT(INT32_MAX);
inline const Element MIN_ELEMENT(INT32_MIN);
inline const Element DEFAULT_ELEMENT(0);

} // namespace ds_imp

namespace std {
    template <>
    struct hash<ds_imp::Element> {
        size_t operator()(const ds_imp::Element& p) const noexcept {
            return hash<decltype(p.get())>()(p.get());
        }
    };
}