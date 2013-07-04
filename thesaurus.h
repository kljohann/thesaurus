#pragma once

#include <unordered_map>
#include <vector>
#include <string>

class Definition
{
public:
	Definition(std::string def) :
		definition(def), synonyms() {}
	Definition(std::string def, std::vector<std::string> const& syns) :
		definition(def), synonyms(syns) {}

	std::string definition;
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
