#include "jsonData.h"

JSONData *JSONData::msInstance = NULL;

JSONData *JSONData::getInstance()
{
	assert(msInstance != NULL);
	return msInstance;
}

void JSONData::init(const char *str)
{
	msInstance = new JSONData(str);
}

void JSONData::cleanup()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = NULL;
	}
}

rapidjson::Document &JSONData::getJSON()
{
	return mDoc;
}

JSONData::JSONData(const char *str)
{
	//open str
	//read all contents into a char *
	//parse *that* char pointer
	std::ifstream fin(str);
	std::string line = "";
	std::stringstream json;
	while(std::getline(fin, line))
	{
		json << line;
	}
	mDoc.Parse(json.str().c_str());
	fin.close();
}

JSONData::~JSONData()
{
}
