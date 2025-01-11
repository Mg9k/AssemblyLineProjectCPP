#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include "Utilities.h"
#include "Workstation.h"
#include "LineManager.h"

using namespace std;
namespace sdds {
	LineManager::LineManager(const string& file, const vector<Workstation*>& stations) {
        Utilities util;
        ifstream fin(file);
        if (!fin.is_open()) {
			throw "Error: unable to open file";
        }
		string name, record;

        while (!fin.eof()) {
			size_t pos = 0u;
			bool more = true;	
			getline(fin, record);

			name = util.extractToken(record, pos, more);
			auto iter = find_if(stations.begin(), stations.end(), [name](const Workstation* ws) -> bool {
				return (name == ws->getItemName());
			});

			if (iter == stations.end())
				throw "Error: Item does not exist";
			else
				m_activeLine.push_back(*iter);

			if (more) {
				name = util.extractToken(record, pos, more);
				iter = find_if(stations.begin(), stations.end(), [name](const Workstation* ws) -> bool {
					return (name == ws->getItemName());
				});

				if (iter == stations.end())
					throw "Error: Next item does not exist";
				else
					m_activeLine.back()->setNextStation(*iter);
			}
			else
				m_activeLine.back()->setNextStation(nullptr);
        }
        fin.close();

		m_cntCustomerOrder = g_pending.size();
		m_firstStation = m_activeLine.front();
	}

	void LineManager::reorderStations() {
		string item;
		for (size_t i = 1; i < m_activeLine.size(); i++)
		{
			auto iter = find_if(m_activeLine.begin(), m_activeLine.end(), [&item, i](const Workstation* ws)-> bool {
				if (i == 1) {
					if (ws->getNextStation() == nullptr) {
						item = ws->getItemName();
						return true;
					}
				}
				if (ws->getNextStation()->getItemName() == item) {
					item = ws->getItemName();
					return true;
				}
				return false;
			});
			iter_swap(iter, m_activeLine.end() - i);
		}
		m_firstStation = m_activeLine.front();

		for (size_t i = 0; i < m_activeLine.size() - 1; i++) {
			m_activeLine[i]->setNextStation(m_activeLine[i+1]);
		}
	}

	bool LineManager::run(ostream& os)
	{
		static int iterCount = 0;
		iterCount++;
		os << "Line Manager Iteration: " << iterCount << endl;
		
		if (!g_pending.empty()) {
			*m_firstStation += move(g_pending.front());     
			g_pending.pop_front(); 
		}

		for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* ws) -> void {
			ws->fill(os);
		});
		for_each(m_activeLine.begin(), m_activeLine.end(), [](Workstation* ws) -> void {
			ws->attemptToMoveOrder();
		});
		return (g_completed.size() + g_incomplete.size() == m_cntCustomerOrder);
	}

	void LineManager::display(ostream& os) const {
		for_each(m_activeLine.begin(), m_activeLine.end(), [&os](const Workstation* ws)->void {
			ws->display(os);
		});
	}
};