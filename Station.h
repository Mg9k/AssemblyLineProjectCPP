// This module represents individual stations that fill out a designated item type
// Ex. a station for wardrobes.

#ifndef SDDS_STATION_H
#define SDDS_STATION_H
#include <string>

namespace sdds {
	class Station {
		int m_stationId;
		std::string m_itemName;
		std::string m_description;
		size_t m_serial;
		size_t m_quantity;
		static size_t m_widthField;
		static int id_generator;
	public:
		Station(const std::string& str);
		const std::string& getItemName() const;
		size_t getNextSerialNumber();
		size_t getQuantity() const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
	};

}

#endif