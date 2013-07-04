#include "thesaurus.h"

#include <iostream>

int main(int argc, char *argv[])
{
	Index idx("/usr/share/myspell/dicts/th_de_DE_v2.idx");
	std::cout << idx.lookup("abbauen") << std::endl;
	return 0;
}
