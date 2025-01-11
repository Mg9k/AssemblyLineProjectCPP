#include <string>
#include "Utilities.h"

using namespace std;
namespace sdds {
	char Utilities::m_delimiter;

	void Utilities::setFieldWidth(size_t newWidth) {
		m_widthField = newWidth;
	}

	size_t Utilities::getFieldWidth() const	{
		return m_widthField;
	}

	// Extracts a token from the current line
	string Utilities::extractToken(const string& str, size_t& next_pos, bool& more) {
		if (str[next_pos] == m_delimiter) {
			more = false;
			throw "Error: Delimiter found at next_pos";
		}
		else {
			size_t tokenEnd = str.find_first_of(m_delimiter + '\0', next_pos);
			string token = str.substr(next_pos, tokenEnd - next_pos);
			next_pos = tokenEnd + 1;
			if (tokenEnd == string::npos)
				more = false;

			if (token[0] == ' ') {
				size_t spaceiter = token.find_first_not_of(' ');
				token.erase(0, spaceiter);
			}
			if (token[token.length() - 1] == ' ') {
				int spaceiter = 0;
				while (token[token.length() - 1 - spaceiter] == ' ') {
					spaceiter++;
				}
				token.erase(token.length() - spaceiter, spaceiter);
			}
			if (m_widthField < token.length())
				setFieldWidth(token.length());

			return token;
		}
	}

	void Utilities::setDelimiter(char newDelimiter) {
		Utilities::m_delimiter = newDelimiter;
	}

	char Utilities::getDelimiter() {
		return Utilities::m_delimiter;
	}
}