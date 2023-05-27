#ifndef SDDS_UTILITIES_H
#define SDDS_UTILITIES_H
#include <string>

namespace sdds {
   class Utilities {
   private:
      size_t m_widthField { 1 }; // specifies length of the token extracted; used for display purposes; default value is 1.
      static char m_delimiter; // separates the tokens in any given string object. All Utilities objects in the system share the same delimiter.
   public:      
      std::string& cleanString(std::string& str); // remove leading and trailing white spaces
      void setFieldWidth(size_t newWidth); // sets the field width of the current object to the value of parameter newWidth
      size_t getFieldWidth() const; // returns the field width of the current object
      std::string extractToken(const std::string& str, size_t& next_pos, bool& more); // extracts a token from string referred to by the first parameter.
      static void setDelimiter(char newDelimiter); //  sets the delimiter for this class to the character received
      static char getDelimiter(); // returns the delimiter for this class.      
   };   
}
#endif // !SDDS_UTILITIES_H