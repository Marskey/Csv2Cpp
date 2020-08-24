#include "template.h"
#include "csv.hpp"
namespace Csv {
  bool CConditionConfig::Load(const char* pFileName) {
    std::ifstream file(pFileName);
      if (!file.is_open()) {
        return false;
      }
      this->Clear();
      csv::CSVReader csvReader(pFileName);
      file.close();
      
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
      std::vector<std::string> vecValues = split(strValues, '|');
      for (auto& value : vecValues) {
        data.Values.emplace_back(atoi(value.c_str()));
      }
      data.TableType = (*iter)["TableType"].get<int32_t>;
    }
    return true;
  }
  bool CConditionConfig::Reload(const char* pFileName) {
    return Load(pFileName);
  }
  bool CConditionConfig::Clear() {
    m_mapData.clear();
  }
}
