#include "thesaurus.h"

#include <utility>

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

Thesaurus::Thesaurus(std::string const& indexpath, std::string const& datapath) :
	mIndex(indexpath), mFile(datapath)
{
	if (!mFile) {
		throw std::runtime_error("Could not open data file: " + datapath);
	}
	if (!std::getline(mFile, mEncoding)) {
		throw std::runtime_error("Could not read encoding.");
	}
}

std::vector<Definition> Thesaurus::lookup(std::string const& name)
{
	size_t offset = mIndex.lookup(name);
	if (offset == 0) {
		return {};
	}

	if (!mFile.seekg(offset)) {
		throw std::runtime_error("Failed to seek to offset provided by index.");
	}

	std::string line;
	if (!std::getline(mFile, line)) {
		throw std::runtime_error("Could not read from data file.");
	}

	std::vector<Definition> results;
	size_t const delim = line.rfind("|");
	size_t const count = std::stoul(line.substr(delim + 1, line.size() - delim - 1));
	results.reserve(count);

	for (size_t ii = count; ii--; ) {
		std::string definition, category;
		std::vector<std::string> synonyms;

		if (!std::getline(mFile, category, '|')) {
			throw std::runtime_error("Could not read part of speech.");
		}

		if (!std::getline(mFile, definition, '|')) {
			throw std::runtime_error("Could not read definition.");
		}

		if (!std::getline(mFile, line)) {
			throw std::runtime_error("Could not read synonyms.");
		}

		line.push_back('|');

		size_t prev = 0, pos = 0;
		while ((pos = line.find("|", prev)) != std::string::npos)
		{
			synonyms.emplace_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}

		results.emplace_back(definition, synonyms, category);
	};

	return results;
}
