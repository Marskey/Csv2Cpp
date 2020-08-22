#include "template.h"
#include "csv.hpp"
namespace Csv {
  bool CConditionConfig::Load(const char* pFileName) {
    std::ifstream file(pFileName);
    if (!file.is_open()) {
        return false;
    }
    this->Clear();
    aria::csv::CsvParser parser(file);
    file.close();
    auto iter = parser.begin();
    for (; iter != parser.end(); ++iter) {
        if (iter.current_row() <= 3) {
            continue;
        }

        for (const auto& field : *iter) {
        }
    }
  }
}
