#include <iostream>
#include "csv.hpp"
#include "fmt/printf.h"

class Formater
{
public:
    Formater() { };
    ~Formater() { };

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
        std::cout<< "field type: " << csvType << " is not recognized" << std::endl;
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
    
    Formater formater;
    // 头文件
    formater.Append("#pragam once\n");
    formater.Append("#include <string>\n");
    formater.Append("#include <unordered_map>\n");


    formater.Append("namespace %s {\n", nameSpace);
    formater.Indent();
    formater.Append("using Index = int32_t;\n");
    formater.Append(
        "class C%sConfig {\n"
        "public:\n"
    , csvName);
    formater.Indent();

    std::vector<std::string> fieldNames;
    std::vector<std::string> fieldTypes;
    std::vector<std::string> fieldDescs;

    size_t fieldCount = 0;
    auto iter = parser.begin();
    for (; iter != parser.end(); ++iter) {
        fieldCount = (*iter).size();
        // 字段名
        if (iter.current_row() == 1) {
            for (const auto& fieldName : *iter) {
                fieldNames.emplace_back(fieldName);
            }
        // 字段类型
        } else if (iter.current_row() == 2) {
            for (const auto& fieldType : *iter) {
                fieldTypes.emplace_back(fieldType);
            }
        // 字段说明
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

    // 最多两个idx
    std::vector<std::string> vecIndexs;
    vecIndexs.reserve(2);

    // 生成结构体
    std::string structName = fmt::format("{0}Data", csvName);
    formater.Append("Struct %s\n", structName);
    formater.Append("{\n");
    formater.Indent();
    for (int i = 0; i < fieldCount; ++i) {
        formater.Append("// %s\n", fieldDescs[i]);
        std::string fieldName;
        if (!SnakeCaseToCamelCase(fieldNames[i], &fieldName)) {
            std::cout << "Error: field name to camelCase failed" << std::endl;
        }
        std::string cppType = GetCppTypeName(fieldTypes[i]);
        if (cppType == "Index") {
            vecIndexs.emplace_back(fieldName);
        }
        formater.Append("%s %s;\n", cppType, fieldName);
    }
    formater.Outdent();
    formater.Append("}\n");
    formater.Outdent();

    // 生成接口
    formater.Append("public:\n");
    formater.Indent();

    formater.Append("bool Load(const char* pFileName);\n");
    formater.Append("bool Reload(const char* pFileName);\n");

    if (vecIndexs.size() == 1) {
        formater.Append("const %s& GetData(int32_t n%s);\n", structName, vecIndexs[0]);
    } else if (vecIndexs.size() == 2) {
        formater.Append("const %s& GetData(int32_t n%s, int32_t n%s);\n", structName, vecIndexs[0], vecIndexs[1]);
    } else {
        std::cout<< "index count must be 1 or 2. current is " << vecIndexs.size() << std::endl;
        return 0;
    }

    formater.Outdent();
    formater.Append("private:\n");
    formater.Indent();
    formater.Append("std::unordered_map<int64_t, %s> m_mapData\n", structName);
    formater.Outdent();
    formater.Append("};\n");
    formater.Outdent();
    formater.Append("};\n");

    std::cout << formater.getBuf() << std::endl;
    return 0;
}
