#include <iostream>

#include <filesystem>
#include "csv.hpp"
#include "fmt/printf.h"
#include "Utils.hpp"

#define ERR_COUT std::cout << "\033[31m[  ERROR  ] \033[0m" 
#define NAME_SPACE_NAME "Csv"
#define OUTPUT_FILE_NAME "CsvConfigInsts"
#define HEADER_COUNT 3
// base on zero
#define HEADER_ROW 0

std::string g_csvPath = "";
std::string g_outputPath = "";

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
        ret = "INDEX_TYPE_";
    } else {
        ERR_COUT << "field type: " << csvType << " is not recognized" << std::endl;
    }
    return ret;
}

bool ParseArguments(int argc, char* argv[]) {
    if (argc != 3) {
        ERR_COUT << "Argument must be two." << std::endl;
        return false;
    }

    g_csvPath = argv[1];
    const std::filesystem::path csvPath(g_csvPath);
    if (!is_directory(csvPath)) {
        ERR_COUT << "Csvpath cannot be a file!" << std::endl;
        return false;
    }

    g_outputPath = argv[2];
    const std::filesystem::path outputPath(g_outputPath);
    if (!is_directory(outputPath)) {
        ERR_COUT << "OutputPath cannot be a file!" << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    //ParseArguments
    if (!ParseArguments(argc, argv)) {
        return 1;
    }

    // gen file
    Formater headerFmt;
    Formater sourceFmt;
    sourceFmt.Append("#include \"{}.h\"\n", OUTPUT_FILE_NAME);
    sourceFmt.Append("#include \"csv.hpp\"\n");
    sourceFmt.Append("namespace {} {{\n", NAME_SPACE_NAME);
    sourceFmt.Indent();

    headerFmt.Append("#pragma once\n");
    headerFmt.Append("#include <string>\n");
    headerFmt.Append("#include <vector>\n");
    headerFmt.Append("#include <map>\n");
    headerFmt.Append("#include <sstream>\n");

    headerFmt.Append("namespace {} {{\n", NAME_SPACE_NAME);
    headerFmt.Indent();
    // define
    headerFmt.Append("#define MAKE_UINT64(hi, lo) ((uint64_t)(((uint32_t)(lo) & 0xffffffffUL) | ((uint64_t)(((uint32_t)(hi)) & 0xffffffffUL)) << 32))\n");
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
    headerFmt.Append("\n");
    headerFmt.Append("using INDEX_TYPE_ = int32_t;\n");

    std::vector<std::string> fileList;
    CUtils::GetFileList(g_csvPath, fileList);

    int nSuccessCount = 0;
    for (const auto& filePath : fileList) {
        std::string ext = "";
        std::string csvName = filePath;
        std::string::size_type posDot = filePath.find_last_of('.');
        if (posDot != std::string::npos) {
            std::string::size_type posSpe = filePath.find_last_of('/', posDot);
            if (posSpe == std::string::npos) {
                posSpe = 0;
            }

            ext = filePath.substr(posDot + 1);
            csvName = filePath.substr(posSpe + 1, posDot - posSpe - 1);
        }

        std::for_each(ext.begin(), ext.end(), [](auto& ch) {
            ch = tolower(ch);
        });

        if (ext != "csv") {
            continue;
        }

        std::string csvFilePath = filePath;

        std::ifstream csvFile(filePath, std::ifstream::in);
        if (!csvFile.is_open()) {
            ERR_COUT << "Can not open file: " << filePath << std::endl;
            continue;
        }

        std::vector<std::string> csvHeadRows;
        int r = 0;
        for (std::string csvRow; std::getline(csvFile, csvRow) && r < HEADER_COUNT; ++r) {
            csvHeadRows.emplace_back(csvRow);
        }
        csvFile.close();

        if (csvHeadRows.size() < HEADER_COUNT) {
            ERR_COUT << "Header row must be HEADER_COUNT. csv name is: " << csvFilePath << std::endl;
            continue;
        }

        std::vector<std::string> fieldNames = CUtils::Split(csvHeadRows[0], ',');
        std::vector<std::string> fieldTypes = CUtils::Split(csvHeadRows[1], ',');
        std::vector<std::string> fieldDescs = CUtils::Split(csvHeadRows[2], ',');

        if (fieldNames.size() == 0) {
            ERR_COUT << "Field count is zero. csv name is: "<< csvFilePath << std::endl;
            continue;
        }

        if (fieldNames.size() != fieldTypes.size()
            && fieldNames.size() != fieldDescs.size()) {
            ERR_COUT << "Field name, field type and field desc count not the same. csv name is: "<< csvFilePath << std::endl;
            continue;
        }

        // start gen config
        std::string className = fmt::format("{}Inst", csvName);
        std::string structName = fmt::format("Data");

        headerFmt.Append("\n");
        headerFmt.Append("// ================== {0}.csv start ==================\n", csvName);
        sourceFmt.Append("\n");
        sourceFmt.Append("// ================== {0}.csv start ==================\n", csvName);
        headerFmt.Append(
            "class {0}\n"
            "{{\n"
            , className
        );
        headerFmt.Indent();
        headerFmt.Append("CSV_DECLARE_SINGLETON({0})\n", className);
        headerFmt.Outdent();

        // struct start
        headerFmt.Append("public:\n", csvName);
        headerFmt.Indent();

        std::vector<std::string> vecIndexs;
        vecIndexs.reserve(2);

        headerFmt.Append("struct {}\n", structName);
        headerFmt.Append("{{\n");
        headerFmt.Indent();

        for (int i = 0; i < fieldNames.size(); ++i) {
            headerFmt.Append("// {}\n", fieldDescs[i]);
            std::string fieldName;
            if (!CUtils::SnakeCaseToCamelCase(fieldNames[i], &fieldName)) {
                std::cout << "Error: field name to camelCase failed" << std::endl;
            }
            std::string cppType = GetCppTypeName(fieldTypes[i]);
            if (cppType == "INDEX_TYPE_") {
                vecIndexs.emplace_back(fieldName);
            }
            headerFmt.Append("{0} {1};\n", cppType, fieldName);
        }
        headerFmt.Outdent();
        headerFmt.Append("}};\n");
        headerFmt.Append("using ContainerType = std::map<uint64_t, Data>;\n");
        headerFmt.Outdent();
        // data end

        // public function start
        headerFmt.Append("public:\n");
        headerFmt.Indent();

        headerFmt.Append("bool Load(const std::string& filePath);\n");
        sourceFmt.Append("bool {0}::Load(const std::string& filePath) {{\n", className);
        sourceFmt.Indent();
        sourceFmt.Append("this->Clear();\n\
csv::CSVFormat format;\n\
format.delimiter({{ ',' }}).quote('\"').header_row({0}).detect_bom(true);\n", HEADER_ROW);
        sourceFmt.Append("try {{\n");
        sourceFmt.Indent();
        sourceFmt.Append("csv::CSVReader csvReader(filePath, format);\n\
\n\
auto iter = csvReader.begin();\n");
        if (HEADER_COUNT - (HEADER_ROW + 1) > 0) {
            sourceFmt.Append("//remove header rows.\nfor (int i = 0; i < {0}; ++i, ++iter);\n", HEADER_COUNT - (HEADER_ROW + 1));
        }
        sourceFmt.Append("for (; iter != csvReader.end(); ++iter) {{\n");
        sourceFmt.Indent();
        sourceFmt.Append("{} data;\n", structName);

        for (int i = 0; i < fieldNames.size(); ++i) {
            std::string fname = fieldNames[i];
            if (fieldTypes[i] == "int[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, ';');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(atoi(value.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "int[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> _vecSubValues = _Internal::split(value, ';');\n");
                sourceFmt.Append("std::vector<int32_t> vecSub{0};\n", fname);
                sourceFmt.Append("for (auto& subValue : _vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("vecSub{0}.emplace_back(atoi(subValue.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Append("data.{0}.emplace_back(vecSub{0});\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "int64[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, ';');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(std::stoll(value.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "int64[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> _vecSubValues = _Internal::split(value, ';');\n");
                sourceFmt.Append("std::vector<int64_t> vecSub{0};\n", fname);
                sourceFmt.Append("for (auto& subValue : _vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("vecSub{0}.emplace_back(std::stoll(subValue.c_str()));\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Append("data.{0}.emplace_back(vecSub{0});\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "string[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, ';');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(value.c_str());\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "string[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> _vecSubValues = _Internal::split(value, ';');\n");
                sourceFmt.Append("data.{0}.emplace_back(_vecSubValues);\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "bool[]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, ';');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("data.{0}.emplace_back(value == \"true\");\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else if (fieldTypes[i] == "bool[][]") {
                sourceFmt.Append("std::string str{0} = (*iter)[\"{0}\"].get<std::string>();\n", fname);
                sourceFmt.Append("std::vector<std::string> vecStr{0} = _Internal::split(str{0}, '|');\n", fname);
                sourceFmt.Append("for (auto& value : vecStr{0}) {{\n", fname);
                sourceFmt.Indent();
                sourceFmt.Append("std::vector<std::string> _vecSubValues = _Internal::split(value, ';');\n");
                sourceFmt.Append("std::vector<bool> vecSub{0};\n", fname);
                sourceFmt.Append("for (auto& subValue : _vecSubValues) {{\n");
                sourceFmt.Indent();
                sourceFmt.Append("vecSub{0}.emplace_back(subValue == \"true\");\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
                sourceFmt.Append("data.{0}.emplace_back(vecSub{0});\n", fname);
                sourceFmt.Outdent();
                sourceFmt.Append("}}\n");
            } else {
                sourceFmt.Append("data.{0} = (*iter)[\"{0}\"].get<{1}>();\n", fieldNames[i], GetCppTypeName(fieldTypes[i]));
            }
        }

        if (vecIndexs.size() == 1) {
            sourceFmt.Append("m_mapData[data.{0}] = data;\n", vecIndexs[0]);
        } else if (vecIndexs.size() == 2) {
            sourceFmt.Append("uint64_t key = MAKE_UINT64(data.{0}, data.{1});\n", vecIndexs[0], vecIndexs[1]);
            sourceFmt.Append("m_mapData[key] = data;\n");
        } else {
            std::cout << "index count must be 1 or 2. current is " << vecIndexs.size() << ". csv name is: "<< csvFilePath<< std::endl;
            continue;
        }

        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");
        // try catch
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n\
catch (std::exception& ex) {{\n");
        sourceFmt.Indent();
        sourceFmt.Append("return false;\n");
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");
        sourceFmt.Append("return true;\n");
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");

        headerFmt.Append("void Clear();\n");
        sourceFmt.Append("void {0}::Clear() {{\n", className);
        sourceFmt.Indent();
        sourceFmt.Append("m_mapData.clear();\n");
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");

        if (vecIndexs.size() == 1) {
            headerFmt.Append("const {}* GetData(INDEX_TYPE_ n{});\n", structName, vecIndexs[0]);
            sourceFmt.Append("const {0}::{1}* {0}::GetData(INDEX_TYPE_ n{2}) {{\n", className, structName, vecIndexs[0]);
            sourceFmt.Indent();
            sourceFmt.Append("auto it = m_mapData.find(n{0});\n\
if (it != m_mapData.end()) {{\n\
  return &it->second;\n\
}}\n\
return nullptr;\n", vecIndexs[0]);
            sourceFmt.Outdent();
            sourceFmt.Append("}}\n");
        } else if (vecIndexs.size() == 2) {
            headerFmt.Append("const {}* GetData(INDEX_TYPE_ n{}, INDEX_TYPE_ n{});\n", structName, vecIndexs[0], vecIndexs[1]);
            sourceFmt.Append("const {0}::{1}* {0}::GetData(INDEX_TYPE_ n{2}, INDEX_TYPE_ n{3}) {{\n", className, structName, vecIndexs[0], vecIndexs[1]);
            sourceFmt.Indent();
            sourceFmt.Append("uint64_t key = MAKE_UINT64(n{0}, n{1});\n\
auto it = m_mapData.find(key);\n\
if (it != m_mapData.end()) {{\n\
  return &it->second;\n\
}}\n\
return nullptr;\n", vecIndexs[0], vecIndexs[1]);
            sourceFmt.Outdent();
            sourceFmt.Append("}}\n");
        } else {
            std::cout << "index count must be 1 or 2. current is " << vecIndexs.size() << ". csv name is: "<< csvFilePath<< std::endl;
            continue;
        }

        headerFmt.Append("const ContainerType& GetAllData();\n");
        sourceFmt.Append("const {0}::ContainerType& {0}::GetAllData() {{\n", className);
        sourceFmt.Indent();
        sourceFmt.Append("return m_mapData;\n");
        sourceFmt.Outdent();
        sourceFmt.Append("}}\n");

        // public function end
        headerFmt.Outdent();
        headerFmt.Append("private:\n");
        headerFmt.Indent();
        headerFmt.Append("ContainerType m_mapData;\n", structName);
        headerFmt.Outdent();
        headerFmt.Append("}};\n"); // csv class def end

        headerFmt.Append("// ================== {0}.csv end ==================\n", csvName);
        sourceFmt.Append("// ================== {0}.csv end ==================\n", csvName);
        ++nSuccessCount;
        std::cout << "\033[92m[ GEN     ] \033[0m" << " CSV: " << csvName << std::endl;
    }

    headerFmt.Append("namespace _Internal {{");
    headerFmt.Indent();
    // split fuc
    headerFmt.Append("\n");
    headerFmt.Append("static std::vector<std::string> split(const std::string& s, const char delim) {{\n");
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
    headerFmt.Outdent();
    headerFmt.Append("}}\n");

    // header namespace
    headerFmt.Outdent();
    headerFmt.Append("}};\n");

    // source namespace
    sourceFmt.Outdent();
    sourceFmt.Append("}}\n");

    std::string headerFilePath = fmt::format("{}/{}.h", g_outputPath, OUTPUT_FILE_NAME);
    std::string sourceFilePath = fmt::format("{}/{}.cpp", g_outputPath, OUTPUT_FILE_NAME);
    std::fstream fs(headerFilePath.c_str(), std::fstream::out);
    fs.write(headerFmt.getBuf().c_str(), headerFmt.getBuf().size());
    fs.close();
    fs.open(sourceFilePath.c_str(), std::fstream::out);
    fs.write(sourceFmt.getBuf().c_str(), sourceFmt.getBuf().size());
    fs.close();
    if (nSuccessCount == fileList.size()) {
        std::cout << "\033[92m[   DONE  ] \033[0m"
        << " DONE: " << nSuccessCount << " Total: " << fileList.size() << std::endl;
    } else {
        std::cout << "\033[31m[   DONE  ] \033[0m"
        << " DONE: " << nSuccessCount << " Total: " << fileList.size() << std::endl;
    }
    return 0;
}
