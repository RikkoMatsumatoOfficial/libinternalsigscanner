#include "all.h"
#include "pattern_batch.h"
#include "range.h"
#include "sig_scanner.h"
#include <iostream>
using namespace std;
namespace memory
{
	void pattern_batch::add(std::string name, pattern pattern, std::function<void(handle)> callback)
	{
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	void pattern_batch::run(range region, bool report_found)
	{
		bool all_found = true;
		for (auto& entry : m_entries)
		{
			bool entry_found = true;
			if (auto result = region.scan(entry.m_pattern))
			{
				if (entry.m_callback)
				{
					if (report_found) {
						cout << "Founded!!!" << endl;
					}
					 
					std::invoke(entry.m_callback, result);
				}
				else
				{
					all_found = false;
					entry_found = false;
				}
			}
			else
			{
				all_found = false;
				entry_found = false;
			}

			if (!entry_found)
				std::cout << "[SigScanner] Failed to find '" << entry.m_name << "'.";
		}

		m_entries.clear();
		if (!all_found)
		{
			return;
		}
	}
}
