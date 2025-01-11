#include <iostream>
#include <iomanip>
#include <utility>
#include "CustomerOrder.h"
#include "Utilities.h"
#include "Station.h"

using namespace std;
namespace sdds {
	size_t CustomerOrder::m_widthField = 0;

	CustomerOrder::CustomerOrder(){
		m_cntItem = 0;
		m_lstItem = nullptr;
	}

	CustomerOrder::CustomerOrder(const string& str) {
		m_cntItem = 0;
		Utilities util;
		size_t pos = 0u;
		bool more = true;
		m_lstItem = nullptr;
		m_name = util.extractToken(str, pos, more);
		m_product = util.extractToken(str, pos, more);

		size_t tempPos = pos;
		while (more) {
			util.extractToken(str, tempPos, more);
			m_cntItem++;
		}
		more = true;

		m_lstItem = new Item*[m_cntItem] {};

		for (size_t i = 0; i < m_cntItem; i++) {
			m_lstItem[i] = new Item(util.extractToken(str, pos, more));
		}

		if (CustomerOrder::m_widthField < util.getFieldWidth())
			CustomerOrder::m_widthField = util.getFieldWidth();
	}

	CustomerOrder::CustomerOrder(const CustomerOrder& other) {
		throw "Error: Class does not allow copy operations";
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept {
		*this = move(other);
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
		if (this != &other) {
			for (size_t i = 0; i < m_cntItem; i++)
				delete m_lstItem[i];
			delete[] m_lstItem;

			m_name = other.m_name;
			m_product = other.m_product;
			m_cntItem = other.m_cntItem;

			m_lstItem = other.m_lstItem;
			other.m_lstItem = nullptr;
		}
		return *this;
	}

	CustomerOrder::~CustomerOrder() {
		if (m_lstItem != nullptr) {
			for (size_t i = 0; i < m_cntItem; i++)
				delete m_lstItem[i];
		}
		
		delete[] m_lstItem;
	}

	bool CustomerOrder::isOrderFilled() const {
		for (size_t i = 0; i < m_cntItem; i++) {
			if (!m_lstItem[i]->m_isFilled)
				return false;
		}
		return true;
	}

	bool CustomerOrder::isItemFilled(const string& itemName) const {
		for (size_t i = 0; i < m_cntItem; i++) {
			if (m_lstItem[i]->m_itemName == itemName && !m_lstItem[i]->m_isFilled)
				return false;
		}
		return true;
	}

	void CustomerOrder::fillItem(Station& station, ostream& os) {
		for (size_t i = 0; i < m_cntItem; i++) {
			if (m_lstItem[i]->m_itemName == station.getItemName() && !m_lstItem[i]->m_isFilled) {	
				if (station.getQuantity() > 0) {
					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
					m_lstItem[i]->m_isFilled = true;
					station.updateQuantity();
					os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]\n";
					break;
				}
				else {
					os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]\n";
				}
			}
		}
	}

	void CustomerOrder::display(ostream& os) const {
		os << right << m_name << " - " << m_product << endl;
		for (size_t i = 0; i < m_cntItem; i++) {
			os << right << "[";
			os.width(6);
			os.fill('0');
			os << m_lstItem[i]->m_serialNumber << "] ";
			os.width(CustomerOrder::m_widthField);
			os.fill(' ');
			os << left << m_lstItem[i]->m_itemName << " - ";
			if (m_lstItem[i]->m_isFilled)
				os << "FILLED\n";
			else
				os << "TO BE FILLED\n";
		}
	}
}