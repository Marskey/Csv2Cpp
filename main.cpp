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
        std::string buf = fmt::format(format, std::forward<Args>(args)...);
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
    } else if (csvType == "int[][]") {
        ret = "std::vector<std::vector<int32_t>>";
    } else if (csvType == "int64") {
        ret = "int64_t";
    } else if (csvType == "int64[]") {
        ret = "std::vector<int64_t>";
    } else if (csvType == "int64[][]") {
        ret = "std::vector<std::vector<int64_t>>";
    } else if (csvType == "string") {
        ret = "std::string";
    } else if (csvType == "string[]") {
        ret = "std::vector<std::string>";
    } else if (csvType == "string[][]") {
        ret = "std::vector<std::vector<std::string>>";
    } else if (csvType == "bool") {
        ret = "bool";
    } else if (csvType == "bool[]") {
        ret = "std::vector<bool>";
    } else if (csvType == "bool[][]") {
        ret = "std::vector<std::vector<bool>>";
    } else if (csvType == "index") {
        ret = "Index";
    } else {
        std::cout << "field type: " << csvType << " is not recognized" << std::endl;
    }
    return ret;
}

bool ParseArguments(int argc, char* argv[]) {
    return true;
}

int main(int argc, char* argv[]) {
    //ParseArguments
    if (!ParseArguments(argc, argv)) {
        return 1;
    }

    // gen file
    std::string nameSpace = "Csv";

    Formater headerFmt;
    Formater sourceFmt;
    sourceFmt.Append("#include \"template.h\"\n");
    sourceFmt.Append("#include \"csv.hpp\"\n");
    sourceFmt.Append("namespace {} {{\n", nameSpace);
    sourceFmt.Indent();

    headerFmt.Append("#pragma once\n");
    headerFmt.Append("#include <string>\n");
    headerFmt.Append("#include <vector>\n");
    headerFmt.Append("#include <unordered_map>\n");
    headerFmt.Append("#include <sstream>\n");

    headerFmt.Append("namespace {} {{\n", nameSpace);
    headerFmt.Indent();
    // define
    headerFmt.Append("#define MAKE_UINT64(a, b) (((uint64_t)(a) & 0xffffffffUL) | ((((uint64_t)(b)) & 0xffffffffUL) << 32))\n");
    headerFmt.Append("#define CSV_DECLARE_SINGLETON(__class_name__)\\\n\
public:\\\n\
  static __class_name__& Instance() {{\\\n\
    static __class_name__ instance;\\\n\
    return instance;\\\n\
  }}\\\n\
private:\\\n\
  __class_name__() = default;\\\n\
  ~__class_name__() = default;\\\n\
  __class_name__(const __class_name__&) = default;\\\n\
  __class_name__& operator=(const __class_name__&) = default;\\\n");
    headerFmt.Append("using Index = int32_t;\n");

    for (int i = 0; i < 1; ++i) {
        csv::CSVFormat format;
        format.delimiter({ ',' })
            .quote('"')
            .header_row(0)
            .detect_bom(true);

        std::string csvName = "Condition";
        std::string csvFileName = "Condition.csv";
        csv::CSVReader csvReader("Condition.csv", format);

        std::vector<std::string> fieldNames;
        std::vector<std::string> fieldTypes;
        std::vector<std::string> fieldDescs;

        // col names
        fieldNames = csvReader.get_col_names();
        auto iter = csvReader.begin();
        // second row
        for (auto& fieldType : *iter) {
            fieldTypes.emplace_back(fieldType.get<>());
        }
        ++iter;
        if (iter == csvReader.end()) {
            std::cout << "Error: header row must be 3. csv name is: "<< csvFileName << std::endl;
            continue;
        }
        // third row
        for (auto& fieldDesc : *iter) {
            fieldDescs.emplace_back(fieldDesc.get<>());
        }

        if (fieldNames.size() == 0) {
            std::cout << "Error: field count is zero. csv name is: "<< csvFileName << std::endl;
            continue;
        }

        if (fieldNames.size() != fieldTypes.size()
            && fieldNames.size() != fieldDescs.size()) {
            std::cout << "Error: field name, field type and field desc count not the same. csv name is: "<< csvFileName << std::endl;
            continue;
        }

        // start gen config
        headerFmt.Append("// ================== {0}.csv start ==================\n", csvName);
        sourceFmt.Append("// ================== {0}.csv start ==================\n", csvName);
        headerFmt.Append("\n");
        headerFmt.Append(
            "class C{0}Config\n"
            "{{\n"
            , csvName
        );
        headerFmt.Indent();
        headerFmt.Append("CSV_DECLARE_SINGLETON(C{0}Config)\n", csvName);
        headerFmt.Outdent();
        headerFmt.Append("public:\n", csvName);
        headerFmt.Indent();

        std::vector<std::string> vecIndexs;
        vecIndexs.reserve(2);

        std::string structName = fmt::format("{0}Data", csvName);
        headerFmt.Append("struct {}\n", structName);
        headerFmt.Append("{{\n");
        headerFmt.Indent();

        for (int i = 0; i < fieldNames.size(); ++i) {
            headerFmt.Append("// {}\n", fieldDescs[i]);
            std::string fieldName;
            if (!SnakeCaseToCamelCase(fieldNames[i], &fieldName)) {
                std::cout << "Error: field name to camelCase failed" << std::endl;
            }
            std::string cppType = GetCppTypeName(fieldTypes[i]);
            if (cppType == "Index") {
                vecIndexs.emplace_back(fieldName);
            }
            headerFmt.Append("{0} {1};\n", cppType, fieldName);
        }
        headerFmt.Outdent();
        headerFmt.Append("}};\n");
        headerFmt.Outdent();

        headerFmt.Append("public:\n");
        headerFmt.Indent();

        headerFmt.Append("bool Load(const std::string& filePath);\n");
        sourceFmt.Append("bool C{0}Config::Load(const std::string& filePath) {{\n", csvName);
        sourceFmt.Indent();
        sourceFmt.Append("this->Clear();\n\
csv::CSVReader csvReader(filePath);\n\
\n\
if (csvReader.num_rows < {1}) {{\n\
  return false;\n\
}}\n\
\n\
auto iter = csvReader.begin();\n", csvName, 3);

        sourceFmt.Append("for (int i = 0; iter != csvReader.end(); ++iter, ++i) {{\n");
        sourceFmt.Indent();
        sourceFmt.Append("if (i < {0}) {{\n\
  continue;\n\
}}\n\
{1} data;\n", 3, structName);

        for (int i = 0; i < fieldNames.size(); ++i) {
            std::string fname = fieldNames[i];
            if (fieldTypes[i] == "int[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(atoi(value.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "int[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> vecSubValues = split(value, ';');\n");
                sourceFmt.Append("for (auto& subValue : vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(atoi(subValue.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "int64[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(atoi64(value.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "int64[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> vecSubValues = split(value, ';');\n");
                sourceFmt.Append("for (auto& subValue : vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(atoi64(subValue.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "string[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(value.c_str());\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "string[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> vecSubValues = split(value, ';');\n");
                sourceFmt.Append("for (auto& subValue : vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(subValue.c_str());\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "bool[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(value == \"true\");\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "bool[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>;\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = split(str{0}, '|');\n");
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> vecSubValues = split(value, ';');\n");
                sourceFmt.Append("for (auto& subValue : vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(subValue == \"true\");\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else {
                sourceFmt.Append("data.{0} = (*iter)[\"{0}\"].get<{1}>;\n", fieldNames[i], GetCppTypeName(fieldTypes[i]));
            }
        }

        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");
        sourceFmt.Append("return true;\n");
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");

        headerFmt.Append("bool Clear();\n");
        sourceFmt.Append("bool C{0}Config::Clear() {{\n", csvName);
        sourceFmt.Indent();
        sourceFmt.Append("m_mapData.clear();\n");
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");

        if (vecIndexs.size() == 1) {
            headerFmt.Append("const {}* GetData(Index n{});\n", structName, vecIndexs[0]);
            sourceFmt.Append("const C{0}Config::{1}* C{0}Config::GetData(Index n{2}) {{\n\
auto it = m_mapData.find(nIndex);\n\
if (it != m_mapData.end()) {{\n\
  return &it->second;\n\
}}\n\
return nullptr;\n\
}}\n", csvName, structName, vecIndexs[0]);
        } else if (vecIndexs.size() == 2) {
            headerFmt.Append("const {}* GetData(int32_t n{}, int32_t n{});\n", structName, vecIndexs[0], vecIndexs[1]);
            sourceFmt.Append("const C{0}Config::{1}* C{0}Config::GetData(Index n{2}, Index n{3}) {{\n\
uint16_t key = MAKE_UINT64(n{2}, n{3});\n\
auto it = m_mapData.find(key);\n\
if (it != m_mapData.end()) {{\n\
  return &it->second;\n\
}}\n\
return nullptr;\n\
}}\n", csvName, structName, vecIndexs[0], vecIndexs[1]);
        } else {
            std::cout << "index count must be 1 or 2. current is " << vecIndexs.size() << ". csv name is: "<< csvFileName<< std::endl;
            continue;
        }

        headerFmt.Outdent();
        headerFmt.Append("private:\n");
        headerFmt.Indent();
        headerFmt.Append("std::unordered_map<uint64_t, {}> m_mapData;\n", structName);
        headerFmt.Outdent();
        headerFmt.Append("}};\n"); // csv class def end

        headerFmt.Append("// ================== {0}.csv end ==================\n", csvName);
        sourceFmt.Append("// ================== {0}.csv end ==================\n", csvName);
    }

    // split fuc
    headerFmt.Append("\n");
    headerFmt.Append("static std::vector<std::string> split(const std::string& s, char delim) {{\n");
    headerFmt.Indent();
    headerFmt.Append("std::stringstream ss(s);\n\
std::string item;\n\
std::vector<std::string> elems;\n\
while (std::getline(ss, item, delim)) {{\n\
  elems.emplace_back(item);\n\
}}\n\
return elems;\n");
    headerFmt.Outdent();
    headerFmt.Append("}}\n");

    // header namespace
    headerFmt.Outdent();
    headerFmt.Append("}};\n");

    // source namespace
    sourceFmt.Outdent();
    sourceFmt.Append("}}\n");

    std::fstream fs("../../../template.h", std::fstream::out);
    fs.write(headerFmt.getBuf().c_str(), headerFmt.getBuf().size());
    fs.close();
    fs.open("../../../template.cpp", std::fstream::out);
    fs.write(sourceFmt.getBuf().c_str(), sourceFmt.getBuf().size());
    fs.close();
    return 0;
}
