#include "thesaurus.h"

#include <iostream>

int main(int argc, char *argv[])
{
	Thesaurus thes("/usr/share/myspell/dicts/th_en_US_v2.idx", "/usr/share/myspell/dicts/th_en_US_v2.dat");
	auto defs = thes.lookup("absolute");
	for (auto const& def : defs)
	{
		std::cout << def.definition << " " << def.category << std::endl;
		for (auto const& syn : def.synonyms)
		{
			std::cout << "  - " << syn << std::endl;
		}
	}

	return 0;
}
