#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

#include <boost/utility.hpp>

class Definition
{
public:
	Definition(std::string def, std::string cat = "") :
		definition(def), category(cat), synonyms() {}
	Definition(std::string def, std::vector<std::string> const& syns, std::string cat = "") :
		definition(def), category(cat), synonyms(syns) {}

	std::string getDefinition() const { return definition; }
	std::string getCategory()   const { return category; }
	std::vector<std::string> getSynonyms() const { return synonyms; }

	bool operator==(Definition const& other) const;
	bool operator!=(Definition const& other) const;

private:
	std::string definition, category;
	std::vector<std::string> synonyms;
};

class Index
{
public:
	Index(std::string const& filename);
	size_t lookup(std::string const& name) const;

private:
	std::unordered_map<std::string, size_t> mIndex;
	std::string mEncoding;
};

class Thesaurus : boost::noncopyable
{
public:
	Thesaurus(std::string const& indexpath, std::string const& datapath);

	std::vector<Definition> lookup(std::string const& name);

private:
	Index mIndex;
	std::ifstream mFile;
	std::string mEncoding;
};
