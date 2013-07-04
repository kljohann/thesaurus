#include "thesaurus.h"

#include <utility>
#include <sstream>
#include <stdexcept>

bool Definition::operator==(Definition const& other) const
{
	return (definition == other.definition
		&& category == other.category
		&& synonyms == other.synonyms);
}

bool Definition::operator!=(Definition const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, Definition const& def)
{
	os << def.getDefinition() << " " << def.getCategory() << std::endl;
	for (auto const& syn : def.getSynonyms())
	{
		os << " - " << syn << std::endl;
	}
	return os;
}

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

size_t Index::lookup(std::string const& name) const
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

		if (!std::getline(mFile, line)) {
			throw std::runtime_error("Could not read line.");
		}

		std::istringstream linestream(line);

		if (!std::getline(linestream, category, '|')) {
			throw std::runtime_error("Could not read part of speech.");
		}

		if (!std::getline(linestream, definition, '|')) {
			throw std::runtime_error("Could not read definition.");
		}

		if (std::getline(linestream, line)) {
			line.push_back('|');

			size_t prev = 0, pos = 0;
			while ((pos = line.find("|", prev)) != std::string::npos)
			{
				synonyms.emplace_back(line.substr(prev, pos - prev));
				prev = pos + 1;
			}
		}

		results.emplace_back(definition, synonyms, category);
	};

	return results;
}

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

BOOST_PYTHON_MODULE(thesaurus)
{
  namespace bp = boost::python;
  bp::class_<std::vector<Definition>>("Definitions")
        .def(bp::vector_indexing_suite<std::vector<Definition>>());

  bp::class_<std::vector<std::string>>("Strings")
        .def(bp::vector_indexing_suite<std::vector<std::string>>());

  bp::class_<Thesaurus, boost::noncopyable>("Thesaurus", bp::init<std::string, std::string>())
	  .def("lookup", &Thesaurus::lookup);

  bp::class_<Definition>("Definition", bp::no_init)
	  .def(bp::self_ns::str(bp::self_ns::self))
	  .add_property("definition", &Definition::getDefinition)
	  .add_property("category",   &Definition::getCategory)
	  .add_property("synonyms",   &Definition::getSynonyms);
}
