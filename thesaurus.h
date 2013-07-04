#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

class Definition
{
public:
	Definition(std::string def, std::string cat = "") :
		definition(def), category(cat), synonyms() {}
	Definition(std::string def, std::vector<std::string> const& syns, std::string cat = "") :
		definition(def), category(cat), synonyms(syns) {}

	std::string definition, category;
	std::vector<std::string> synonyms;
};

class Index
{
public:
	Index(std::string const& filename);
	size_t lookup(std::string const& name);

private:
	std::unordered_map<std::string, size_t> mIndex;
	std::string mEncoding;
};

class Thesaurus
{
public:
	Thesaurus(std::string const& indexpath, std::string const& datapath);

	std::vector<Definition> lookup(std::string const& name);

private:
	Index mIndex;
	std::ifstream mFile;
	std::string mEncoding;
};
