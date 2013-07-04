#include "thesaurus.h"

#include <utility>
#include <fstream>

Index::Index(std::string const& filename)
{
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open index file: " + filename);
	}
	if (!std::getline(file, mEncoding)) {
		throw std::runtime_error("Could not read encoding.");
	}

	std::string count;
	if (!std::getline(file, count)) {
		throw std::runtime_error("Could not read index size.");
	}

	mIndex.reserve(std::stoul(count));

	std::string line;
	while (std::getline(file, line))
	{
		size_t const delim = line.rfind("|");

		mIndex.emplace(std::make_pair(
			line.substr(0, delim),
			std::stoul(line.substr(delim + 1, line.size() - delim - 1))));
	}
}

size_t Index::lookup(std::string const& name)
{
	auto const it = mIndex.find(name);
	if (it != mIndex.end()) {
		return it->second;
	}

	// Due to the encoding header there is no offset 0 in mythes files.
	return 0;
}
