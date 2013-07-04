#include "thesaurus.h"

#include <iostream>

int main(int argc, char *argv[])
{
	Thesaurus thes("/usr/share/myspell/dicts/th_de_DE_v2.idx", "/usr/share/myspell/dicts/th_de_DE_v2.dat");
	auto defs = thes.lookup("absolut");
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
