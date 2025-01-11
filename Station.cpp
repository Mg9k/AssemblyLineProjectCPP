#include <string>
#include <iostream>
#include <iomanip>
#include "Station.h"
#include "Utilities.h"

using namespace std;
namespace sdds {
	size_t Station::m_widthField = 0;
	int Station::id_generator = 0;

	// Uses Utilities module to extract and store station information
	Station::Station(const string& str) {
		Utilities util;
		size_t pos = 0u;
		bool more = true;
		Station::id_generator++;
		m_stationId = Station::id_generator;
		m_itemName = util.extractToken(str, pos, more);
		m_serial = atoi(util.extractToken(str, pos, more).c_str());
		m_quantity = atoi(util.extractToken(str, pos, more).c_str());
		if (Station::m_widthField < util.getFieldWidth()) 
			Station::m_widthField = util.getFieldWidth();
		m_description = util.extractToken(str, pos, more);
	}

	const string& Station::getItemName() const {
		return m_itemName;
	}

	size_t Station::getNextSerialNumber() {
		return m_serial++;
	}

	size_t Station::getQuantity() const {
		return m_quantity;
	}

	void Station::updateQuantity() {
		if (m_quantity != 0)
			m_quantity--;
	}

	void Station::display(ostream& os, bool full) const {
		os.fill('0');
		os.width(3);
		os << right << m_stationId << " | ";
		os.fill(' ');
		os.width(Station::m_widthField);
		os << left << m_itemName << " | ";
		os.width(6);
		os.fill('0');
		os << right << m_serial << " | ";
		os.fill(' ');
		if (full) { 
			os.width(4);
			os << right << m_quantity << " | ";
			os << m_description;
		}
		os << endl;
	}
}
