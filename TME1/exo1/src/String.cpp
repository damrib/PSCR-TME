#include "String.h"

namespace sbibl{

    std::size_t length(const char* s) {
        std::size_t len = 0;
        while ( s[len] != '\0') {len += 1;}
        return len;
    }

    char * newcopy(const char* s) {
        std::size_t len = length(s) + 1;
        char * str = new char[len];
        do{
            *str++ = *s;
        } while (*s++);
        return str - len;
    }

    int compare(const String& s1, const String& s2) {
    const char *str1 = s1.get_string();
    const char *str2 = s2.get_string();

    do {
        if ( *str1 < *str2 ) return -1;
        else if (*str1 > *str2) return 1; 
    } while(*++str1 && *++str2);

    return 0;
    }

}

String::String(const char* s) {
    this->str = sbibl::newcopy(s);
}

String::~String() {
    if (this->str) delete[] this->str;
}

String::String(const String& other) {
    this->str = sbibl::newcopy(other.get_string());
}

String & String::operator=(const String& other) {
    if ( this->str ) delete[] this->str;
    this->str = sbibl::newcopy(other.get_string());
    return *this;
}

const char* String::get_string() const {
    return this->str;
}

char String::first_char() const { 
    if (this->str) 
        return this->str[0]; 
    else throw std::invalid_argument("empty String");
}

void String::modify_c(const char c) {
    this->str[0] = c;
}
size_t String::length() const {
    return sbibl::length(this->str);
}

bool operator==(const String& s1, const String& s2) {
    return sbibl::compare(s1, s2) == 0;
}

bool String::operator<(const String& s) const {
    return sbibl::compare(*this, s) < 0;
}

std::ostream & operator<<(std::ostream& os, const String& s) {
    os << s.get_string();
    return os;
}