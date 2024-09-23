#include <cstring>
#include <iostream>

class String {

    char * str = nullptr;

    public:
     
      String (const char*);
      String (const String&);
      String &operator=(String& other);

      const char* get_string() const;

      size_t length() const;
      char first_char() const;
      void modify_c(const char c);

      bool operator<(const String&) const;
      friend bool operator==(const String&, const String&);

      String& operator=(const String& s);

      ~String();

};

std::ostream & operator<<(std::ostream& os, const String& s);

bool operator==(const String& s1, const String& s2);

namespace sbibl{
    std::size_t length(const char*);
    char * newcopy(const char* s);
    int compare(const String& s1, const String& s2);
}