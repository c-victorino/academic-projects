#include <string>
#include <stdexcept>
#include "Utilities.h"

namespace sdds {
   char Utilities::m_delimiter{};

   std::string& Utilities::cleanString(std::string& str) {
      str.erase(0, str.find_first_not_of(" \t\n\r"));
      str.erase(str.find_last_not_of(" \t\n\r") + 1);
      return str;
   }

   void Utilities::setFieldWidth(size_t newWidth) {
      m_widthField = newWidth;
   }

   size_t Utilities::getFieldWidth() const {
      return m_widthField;
   }

   std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
      std::string token{};

      // Check if delimiter is not found at next_pos
      if (str.at(next_pos) != getDelimiter()) {
         // Find the next delimiter starting from next_pos
         int end_pos = str.find(getDelimiter(), next_pos);

         // If a delimiter is found, extract the token
         if (end_pos != std::string::npos) {
            token = str.substr(next_pos, end_pos - next_pos);
            std::string aa = token;
            next_pos = end_pos + 1;
            more = next_pos != std::string::npos;
         }
         // If a delimiter is not found, extract the last token in the string
         else {
            more = false;
            token = str.substr(next_pos, str.length() - next_pos);
         }

         // Remove leading and trailing spaces from the token
         token = cleanString(token);

         // Update the width field if necessary
         if (m_widthField < token.length()) {
            setFieldWidth(token.length());
         }
      }
      // If delimiter is found at next_pos, throw an exception
      else {
         more = false;
         throw std::invalid_argument("extractToken: invalid argument (delimiter is found at next_pos)");
      }
      return token;
   }

   void Utilities::setDelimiter(char newDelimiter) { m_delimiter = newDelimiter; }

   char Utilities::getDelimiter() { return m_delimiter; }
}