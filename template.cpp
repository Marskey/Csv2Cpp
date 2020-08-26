#include "template.h"
#include "csv.hpp"
namespace Csv {
  // ================== Condition.csv start ==================
  bool CConditionConfig::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVReader csvReader(filePath);

    if (csvReader.num_rows < 3) {
      return false;
    }

    auto iter = csvReader.begin();
    for (int i = 0; iter != csvReader.end(); ++iter, ++i) {
      if (i < 3) {
        continue;
      }
      ConditionData data;
      data.Index = (*iter)["Index"].get<Index>;
      data.ConditionID = (*iter)["ConditionID"].get<int32_t>;
      data.Type = (*iter)["Type"].get<int32_t>;
      std::string strValues = (*iter)["Values"].get<std::string>;
      std::vector<std::string> vecStrValues = split(strValues, '|');
      for (auto& value : vecStrValues) {
        data.Values.emplace_back(atoi(value.c_str()));
      }
      std::string strTableType = (*iter)["TableType"].get<std::string>;
      std::vector<std::string> vecStrTableType = split(strTableType, '|');
      for (auto& value : vecStrTableType) {
        std::vector<std::string> vecSubValues = split(value, ';');
        for (auto& subValue : vecSubValues) {
          data.TableType.emplace_back(atoi(subValue.c_str()));
        }
      }
    }
    return true;
  }
  bool CConditionConfig::Clear() {
    m_mapData.clear();
  }
  const CConditionConfig::ConditionData* CConditionConfig::GetData(Index nIndex) {
  auto it = m_mapData.find(nIndex);
  if (it != m_mapData.end()) {
    return &it->second;
  }
  return nullptr;
  }
  // ================== Condition.csv end ==================
}
