#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>

class Definition
{
public:
	Definition(std::string const& def, std::string const& cat = "",
			   std::vector<std::string> const& syns = {}) :
		definition(def), category(cat), synonyms(syns) {}

	Definition(std::string&& def, std::string&& cat,
			   std::vector<std::string>&& syns) :
		definition(def), category(cat), synonyms(syns) {}

	std::string const& getDefinition() const { return definition; }
	std::string const& getCategory()   const { return category; }
	std::vector<std::string> const& getSynonyms() const { return synonyms; }

	bool operator==(Definition const& other) const;
	bool operator!=(Definition const& other) const;

private:
	std::string definition, category;
	std::vector<std::string> synonyms;
};

std::ostream& operator<<(std::ostream& os, Definition const& def);

class Index
{
public:
	Index(std::string const& filename);
	size_t lookup(std::string const& name) const;

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
	Index const mIndex;
	std::ifstream mFile;
	std::string mEncoding;
};
