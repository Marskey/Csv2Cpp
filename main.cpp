#include <iostream>
#include "csv.hpp"
#include "fmt/printf.h"

class Formater
{
public:
    Formater() {};
    ~Formater() {};

    template <typename... Args>
    void Append(const char* format, Args&&... args) {
        std::string buf = fmt::sprintf(format, std::forward<Args>(args)...);
        for (const auto& ch : buf) {
            switch (ch) {
            case '\n':
                m_atStartOfLine = true;
                break;
            default:
                if (m_atStartOfLine) {
                    m_buffer += m_indent;
                    m_atStartOfLine = false;
                }
                break;
            }
            m_buffer += ch;
        }
    }

    void Indent() {
        m_indent += "  ";
    }

    void Outdent() {
        if (m_indent.empty()) {
            return;
        }
        m_indent.resize(m_indent.size() - 2);
    }

    const std::string& getBuf() {
        return m_buffer;
    }

private:
    std::string m_indent = "";
    std::string m_buffer;
    bool m_atStartOfLine = false;
};

bool SnakeCaseToCamelCase(std::string input, std::string* output) {
    output->clear();
    bool after_underscore = false;
    for (size_t i = 0; i < input.size(); ++i) {
        //if (input[i] >= 'A' && input[i] <= 'Z') {
        //  // The field name must not contain uppercase letters.
        //  return false;
        //}
        if (after_underscore) {
            if (input[i] >= 'a' && input[i] <= 'z') {
                output->push_back(input[i] + 'A' - 'a');
                after_underscore = false;
            } else {
                // The character after a "_" must be a lowercase letter.
                return false;
            }
        } else if (input[i] == '_') {
            after_underscore = true;
        } else {
            output->push_back(input[i]);
        }
    }
    if (after_underscore) {
        // Trailing "_".
        return false;
    }
    return true;
}

std::string GetCppTypeName(const std::string& csvFieldType) {
    std::string ret;
    std::string csvType = csvFieldType;
    std::for_each(csvType.begin(), csvType.end(), [](auto& ch) {
        ch = tolower(ch);
                  });
    if (csvType == "int") {
        ret = "int32_t";
    } else if (csvType == "int[]") {
        ret = "std::vector<int32_t>";
    } else if (csvType == "int64") {
        ret = "int64_t";
    } else if (csvType == "int64[]") {
        ret = "std::vector<int64_t>";
    } else if (csvType == "string") {
        ret = "std::string";
    } else if (csvType == "string[]") {
        ret = "std::vector<std::string>";
    } else if (csvType == "bool") {
        ret = "bool";
    } else if (csvType == "bool[]") {
        ret = "std::vector<bool>";
    } else if (csvType == "index") {
        ret = "Index";
    } else {
        std::cout << "field type: " << csvType << " is not recognized" << std::endl;
    }
    return ret;
}

int main(int, char**) {
    std::ifstream file("Condition.csv");
    if (!file.is_open()) {
        std::cout << "cannot find csv" << std::endl;
        return 0;
    }

    aria::csv::CsvParser parser(file);

    std::string nameSpace = "Csv";
    std::string csvName = "Condition";

    Formater headerFmt;
    Formater sourceFmt;
    sourceFmt.Append("#include \"template.h\"\n");
    sourceFmt.Append("#include \"csv.hpp\"\n");
    sourceFmt.Append("namespace %s {\n", nameSpace);
    sourceFmt.Indent();

    headerFmt.Append("#pragma once\n");
    headerFmt.Append("#include <string>\n");
    headerFmt.Append("#include <vector>\n");
    headerFmt.Append("#include <unordered_map>\n");

    headerFmt.Append("namespace %s {\n", nameSpace);
    headerFmt.Indent();
    headerFmt.Append("using Index = int32_t;\n");
    headerFmt.Append(
        "class C%sConfig\n"
        "{\n"
        "public:\n"
        "  C%sConfig() = default;\n"
        "  ~C%sConfig() = default;\n"
        , csvName
        , csvName
        , csvName);

    headerFmt.Indent();

    std::vector<std::string> fieldNames;
    std::vector<std::string> fieldTypes;
    std::vector<std::string> fieldDescs;

    size_t fieldCount = 0;
    auto iter = parser.begin();
    for (; iter != parser.end(); ++iter) {
        fieldCount = (*iter).size();
        if (iter.current_row() == 1) {
            for (const auto& fieldName : *iter) {
                fieldNames.emplace_back(fieldName);
            }
        } else if (iter.current_row() == 2) {
            for (const auto& fieldType : *iter) {
                fieldTypes.emplace_back(fieldType);
            }
        } else if (iter.current_row() == 3) {
            for (const auto& fieldDesc : *iter) {
                fieldDescs.emplace_back(fieldDesc);
            }
        } else {
            break;
        }
    }

    if (fieldCount == 0) {
        std::cout << "Error: field count is zero." << std::endl;
        return 0;
    }

    if (fieldNames.size() != fieldTypes.size()
        && fieldNames.size() != fieldDescs.size()) {
        std::cout << "Error: field name, field type and field desc count not the same." << std::endl;
        return 0;
    }

    std::vector<std::string> vecIndexs;
    vecIndexs.reserve(2);

    std::string structName = fmt::format("{0}Data", csvName);
    headerFmt.Append("struct %s\n", structName);
    headerFmt.Append("{\n");
    headerFmt.Indent();
    for (int i = 0; i < fieldCount; ++i) {
        headerFmt.Append("// %s\n", fieldDescs[i]);
        std::string fieldName;
        if (!SnakeCaseToCamelCase(fieldNames[i], &fieldName)) {
            std::cout << "Error: field name to camelCase failed" << std::endl;
        }
        std::string cppType = GetCppTypeName(fieldTypes[i]);
        if (cppType == "Index") {
            vecIndexs.emplace_back(fieldName);
        }
        headerFmt.Append("%s %s;\n", cppType, fieldName);
    }
    headerFmt.Outdent();
    headerFmt.Append("};\n");
    headerFmt.Outdent();

    headerFmt.Append("public:\n");
    headerFmt.Indent();

    headerFmt.Append("bool Load(const char* pFileName);\n");
    sourceFmt.Append("bool C%sConfig::Load(const char* pFileName) {\n", csvName);
    sourceFmt.Indent();
    sourceFmt.Append("");

    sourceFmt.Outdent();
    sourceFmt.Append("}\n");



    headerFmt.Append("bool Reload(const char* pFileName);\n");

    if (vecIndexs.size() == 1) {
        headerFmt.Append("const %s& GetData(int32_t n%s);\n", structName, vecIndexs[0]);
    } else if (vecIndexs.size() == 2) {
        headerFmt.Append("const %s& GetData(int32_t n%s, int32_t n%s);\n", structName, vecIndexs[0], vecIndexs[1]);
    } else {
        std::cout << "index count must be 1 or 2. current is " << vecIndexs.size() << std::endl;
        return 0;
    }

    headerFmt.Outdent();
    headerFmt.Append("private:\n");
    headerFmt.Indent();
    headerFmt.Append("bool m_bInit = false;\n");
    headerFmt.Append("std::unordered_map<int64_t, %s> m_mapData;\n", structName);
    headerFmt.Outdent();
    headerFmt.Append("};\n");
    headerFmt.Outdent();
    headerFmt.Append("};\n");

    sourceFmt.Outdent();
    sourceFmt.Append("}\n");

    std::fstream fs("../../../template.h", std::fstream::out);
    fs.write(headerFmt.getBuf().c_str(), headerFmt.getBuf().size());
    fs.close();
    fs.open("../../../template.cpp", std::fstream::out);
    fs.write(sourceFmt.getBuf().c_str(), sourceFmt.getBuf().size());
    fs.close();
    return 0;
}
