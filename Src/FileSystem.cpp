
#include "FileSystem.h"

const char* FileSys::ReadINIstring(const char* file, const char* section, const char* key)
{
    inicpp::IniManager f(file);
    std::string str = f[section][key];

    // Allocate memory for the returned C-string
    char* result = new char[str.size() + 1];
    std::strcpy(result, str.c_str());

    return result;

    delete[] str.c_str();
}

int FileSys::ReadINIint(const char* file, const char* section, const char* key)
{
    inicpp::IniManager f(file);
    std::string str = f[section][key];
    return std::stoi(str);
}

float FileSys::ReadINIfloat(const char* file, const char* section, const char* key)
{
    inicpp::IniManager f(file);
    std::string str = f[section][key];

    // Convert the string to float
    float num = std::stof(str);

    // Round the float to two decimal places
    num = round(num * 100.0f) / 100.0f;

    return num;
}

void FileSys::WriteINIstring(const char* file, const char* section, const char* key, const char* value)
{
    inicpp::IniManager f(file);
    f.modify(section, key, value, "");
}

void FileSys::WriteINIint(const char* file, const char* section, const char* key, int value)
{
    inicpp::IniManager f(file);
    f.modify(section, key, value, "");
}

void FileSys::WriteINIfloat(const char* file, const char* section, const char* key, float value)
{
    inicpp::IniManager f(file);
    f.modify(section, key, value, "");
}

const char* FileSys::ReadJSONstring(const char* file, const char* key)
{
    std::ifstream f(file, std::ifstream::binary);

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;

    if (!Json::parseFromStream(readerBuilder, f, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return ""; // return an empty string on error
    }

    std::string str = root[key].asString();

    char* result = new char[str.size() + 1];
    std::strcpy(result, str.c_str());

    return result;

    delete[] str.c_str();
}

int FileSys::ReadJSONint(const char* file, const char* key)
{
    std::ifstream f(file, std::ifstream::binary);

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;

    if (!Json::parseFromStream(readerBuilder, f, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return 0; // return a default value on error
    }

    return root[key].asInt();
}

float FileSys::ReadJSONfloat(const char* file, const char* key)
{
    std::ifstream f(file, std::ifstream::binary);

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;

    if (!Json::parseFromStream(readerBuilder, f, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return 0.0f; // return a default value on error
    }

    float num = root[key].asFloat();
    return std::round(num * 100.0f) / 100.0f; // round to two decimal places
}

void FileSys::WriteJSONstring(const char* file, const char* key, const char* value)
{
    if (!file || !key || !value) {
        std::cerr << "Invalid arguments" << std::endl;
        return;
    }

    Json::Value root;
    std::ifstream ifs(file, std::ifstream::binary);
    if (ifs.is_open()) {
        Json::CharReaderBuilder readerBuilder;
        std::string errs;
        Json::parseFromStream(readerBuilder, ifs, &root, &errs);
    }
    ifs.close();

    root[key] = value;

    Json::StreamWriterBuilder writer;
    std::ofstream f(file, std::ofstream::binary);
    if (!f.is_open()) {
        std::cerr << "Failed to open file: " << file << std::endl;
        return;
    }

    f << Json::writeString(writer, root);
}

void FileSys::WriteJSONint(const char* file, const char* key, int value)
{
    if (!file || !key) {
        std::cerr << "Invalid arguments" << std::endl;
        return;
    }

    Json::Value root;
    std::ifstream ifs(file, std::ifstream::binary);
    if (ifs.is_open()) {
        Json::CharReaderBuilder readerBuilder;
        std::string errs;
        Json::parseFromStream(readerBuilder, ifs, &root, &errs);
    }
    ifs.close();

    root[key] = value;

    Json::StreamWriterBuilder writer;
    std::ofstream f(file, std::ofstream::binary);
    if (!f.is_open()) {
        std::cerr << "Failed to open file: " << file << std::endl;
        return;
    }

    f << Json::writeString(writer, root);
}

void FileSys::WriteJSONfloat(const char* file, const char* key, float value)
{
    if (!file || !key) {
        std::cerr << "Invalid arguments" << std::endl;
        return;
    }

    Json::Value root;
    std::ifstream ifs(file, std::ifstream::binary);
    if (ifs.is_open()) {
        Json::CharReaderBuilder readerBuilder;
        std::string errs;
        Json::parseFromStream(readerBuilder, ifs, &root, &errs);
    }
    ifs.close();

    root[key] = value;

    Json::StreamWriterBuilder writer;
    std::ofstream f(file, std::ofstream::binary);
    if (!f.is_open()) {
        std::cerr << "Failed to open file: " << file << std::endl;
        return;
    }

    f << Json::writeString(writer, root);
}