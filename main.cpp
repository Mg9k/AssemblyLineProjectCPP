#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Station.h"
#include "Station.h"
#include "Workstation.h"
#include "Workstation.h"
#include "CustomerOrder.h"
#include "CustomerOrder.h"
#include "Utilities.h"
#include "Utilities.h"
#include "LineManager.h"
#include "LineManager.h"

using namespace std;
using namespace sdds;

template<typename T>
static void loadFromFile(const char*, vector<T>&);
template<typename T>
static void loadFromFile(const char*, vector<T*>&);


int main()
{

	vector<Workstation*> theStations;
	vector<CustomerOrder> theOrders;

	try {
		Utilities::setDelimiter(',');
		loadFromFile("Stations1.txt", theStations);
		Utilities::setDelimiter('|');
		loadFromFile("Stations2.txt", theStations);

		cout << "========================================" << endl;
		cout << "=         Stations (summary)           =" << endl;
		cout << "========================================" << endl;
		for (const auto* station : theStations)
			station->Station::display(cout, false);
		cout << endl << endl;

		cout << "========================================" << endl;
		cout << "=          Stations (full)             =" << endl;
		cout << "========================================" << endl;
		for (const auto* station : theStations)
			station->Station::display(cout, true);
		cout << endl << endl;

		// Select an object and verify all the functionality it working
		cout << "========================================" << endl;
		cout << "=          Manual Validation           =" << endl;
		cout << "========================================" << endl;
		cout << "getItemName(): " << theStations[0]->getItemName() << endl;
		cout << "getNextSerialNumber(): " << theStations[0]->getNextSerialNumber() << endl;
		cout << "getNextSerialNumber(): " << theStations[0]->getNextSerialNumber() << endl;
		cout << "getQuantity(): " << theStations[0]->getQuantity() << endl;
		theStations[0]->updateQuantity();
		cout << "getQuantity(): " << theStations[0]->getQuantity() << endl;
		cout << endl << endl;


		loadFromFile<CustomerOrder>("CustomerOrders.txt", theOrders);

		cout << "========================================" << endl;
		cout << "=                Orders                =" << endl;
		cout << "========================================" << endl;
		for (auto& order : theOrders) {
			order.display(std::cout);
			g_pending.push_back(std::move(order));
		}
		cout << endl << endl;

		cout << "========================================" << endl;
		cout << "=       Display Stations (loaded)      =" << endl;
		cout << "========================================" << endl;
		LineManager lm("AssemblyLine.txt", theStations);
		lm.display(cout);
		cout << endl << endl;

		cout << "========================================" << endl;
		cout << "=      Display Stations (ordered)      =" << endl;
		cout << "========================================" << endl;
		lm.reorderStations();
		lm.display(cout);
		cout << endl << endl;

		cout << "========================================" << endl;
		cout << "=           Filling Orders             =" << endl;
		cout << "========================================" << endl;
		// Run the assembly line until all orders are processed
		while (!lm.run(cout));		
		cout << endl << endl;
	}
	catch (const string& msg) {
		cerr << msg << '\n';
		return 2;
	}

	cout << "========================================" << endl;
	cout << "=      Processed Orders (complete)     =" << endl;
	cout << "========================================" << endl;
	for (const auto& o : g_completed)
		o.display(cout);
	cout << endl << endl;

	cout << "========================================" << endl;
	cout << "=     Processed Orders (incomplete)    =" << endl;
	cout << "========================================" << endl;
	for (const auto& o : g_incomplete)
		o.display(cout);
	cout << endl << endl;

	cout << "========================================" << endl;
	cout << "=          Inventory (full)            =" << endl;
	cout << "========================================" << endl;
	for (const Station* theItem : theStations)
		theItem->display(cout, true);
	cout << endl;

	// Cleanup
	for (auto station : theStations)
		delete station;

	return 0;
}

template<typename T>
static void loadFromFile(const char* filename, vector<T>& theCollection)
{
	if (!filename) {
		throw string("ERROR: No filename provided.");
	}
	ifstream file(filename);
	if (!file)
		throw string("Unable to open [") + filename + "] file.";

	string record;
	while (!file.eof())
	{
		std::getline(file, record);
		T elem(record);
		theCollection.push_back(std::move(elem));
	}

	file.close();
}

template<typename T>
static void loadFromFile(const char* filename, vector<T*>& theCollection)
{
	if (!filename) {
		throw string("ERROR: No filename provided.");
	}
	ifstream file(filename);
	if (!file)
		throw string("Unable to open [") + filename + "] file.";

	string record;
	while (!file.eof())
	{
		std::getline(file, record);
		theCollection.push_back(new T(record));
	}

	file.close();
}
