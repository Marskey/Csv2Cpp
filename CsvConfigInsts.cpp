#include "CsvConfigInsts.h"
#include "csv.hpp"
namespace Csv {

  // ================== OneIndex.csv start ==================
  bool OneIndexInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    try {
      csv::CSVReader csvReader(filePath, format);

      auto iter = csvReader.begin();
      //remove header rows.
      for (int i = 0; i < 2; ++i, ++iter);
      for (; iter != csvReader.end(); ++iter) {
        Data data;
        data.Index = (*iter)["Index"].get<INDEX_TYPE_>();
        data.ConditionID = (*iter)["ConditionID"].get<int32_t>();
        data.Type = (*iter)["Type"].get<int32_t>();
        std::string strIntArray = (*iter)["IntArray"].get<std::string>();
        std::vector<std::string> vecStrIntArray = _Internal::split(strIntArray, ';');
        for (auto& value : vecStrIntArray) {
          data.IntArray.emplace_back(atoi(value.c_str()));
        }
        std::string strIntDulArray = (*iter)["IntDulArray"].get<std::string>();
        std::vector<std::string> vecStrIntDulArray = _Internal::split(strIntDulArray, '|');
        for (auto& value : vecStrIntDulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          std::vector<int32_t> vecSubIntDulArray;
          for (auto& subValue : _vecSubValues) {
            vecSubIntDulArray.emplace_back(atoi(subValue.c_str()));
          }
          data.IntDulArray.emplace_back(vecSubIntDulArray);
        }
        std::string strBoolArray = (*iter)["BoolArray"].get<std::string>();
        std::vector<std::string> vecStrBoolArray = _Internal::split(strBoolArray, ';');
        for (auto& value : vecStrBoolArray) {
          data.BoolArray.emplace_back(value == "true");
        }
        std::string strBoolDulArray = (*iter)["BoolDulArray"].get<std::string>();
        std::vector<std::string> vecStrBoolDulArray = _Internal::split(strBoolDulArray, '|');
        for (auto& value : vecStrBoolDulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          std::vector<bool> vecSubBoolDulArray;
          for (auto& subValue : _vecSubValues) {
            vecSubBoolDulArray.emplace_back(subValue == "true");
          }
          data.BoolDulArray.emplace_back(vecSubBoolDulArray);
        }
        std::string strStringArray = (*iter)["StringArray"].get<std::string>();
        std::vector<std::string> vecStrStringArray = _Internal::split(strStringArray, ';');
        for (auto& value : vecStrStringArray) {
          data.StringArray.emplace_back(value.c_str());
        }
        std::string strStringDulArray = (*iter)["StringDulArray"].get<std::string>();
        std::vector<std::string> vecStrStringDulArray = _Internal::split(strStringDulArray, '|');
        for (auto& value : vecStrStringDulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          data.StringDulArray.emplace_back(_vecSubValues);
        }
        std::string strInt64Array = (*iter)["Int64Array"].get<std::string>();
        std::vector<std::string> vecStrInt64Array = _Internal::split(strInt64Array, ';');
        for (auto& value : vecStrInt64Array) {
          data.Int64Array.emplace_back(std::stoll(value.c_str()));
        }
        std::string strInt64DulArray = (*iter)["Int64DulArray"].get<std::string>();
        std::vector<std::string> vecStrInt64DulArray = _Internal::split(strInt64DulArray, '|');
        for (auto& value : vecStrInt64DulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          std::vector<int64_t> vecSubInt64DulArray;
          for (auto& subValue : _vecSubValues) {
            vecSubInt64DulArray.emplace_back(std::stoll(subValue.c_str()));
          }
          data.Int64DulArray.emplace_back(vecSubInt64DulArray);
        }
        m_mapData[data.Index] = data;
      }
    }
    catch (std::exception& ex) {
      return false;
    }
    return true;
  }
  void OneIndexInst::Clear() {
    m_mapData.clear();
  }
  const OneIndexInst::Data* OneIndexInst::GetData(INDEX_TYPE_ nIndex) {
    auto it = m_mapData.find(nIndex);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  const OneIndexInst::ContainerType& OneIndexInst::GetAllData() {
    return m_mapData;
  }
  // ================== OneIndex.csv end ==================

  // ================== TwoIndex.csv start ==================
  bool TwoIndexInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    try {
      csv::CSVReader csvReader(filePath, format);

      auto iter = csvReader.begin();
      //remove header rows.
      for (int i = 0; i < 2; ++i, ++iter);
      for (; iter != csvReader.end(); ++iter) {
        Data data;
        data.Index = (*iter)["Index"].get<INDEX_TYPE_>();
        data.ConditionID = (*iter)["ConditionID"].get<INDEX_TYPE_>();
        data.Type = (*iter)["Type"].get<int32_t>();
        std::string strIntArray = (*iter)["IntArray"].get<std::string>();
        std::vector<std::string> vecStrIntArray = _Internal::split(strIntArray, ';');
        for (auto& value : vecStrIntArray) {
          data.IntArray.emplace_back(atoi(value.c_str()));
        }
        std::string strIntDulArray = (*iter)["IntDulArray"].get<std::string>();
        std::vector<std::string> vecStrIntDulArray = _Internal::split(strIntDulArray, '|');
        for (auto& value : vecStrIntDulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          std::vector<int32_t> vecSubIntDulArray;
          for (auto& subValue : _vecSubValues) {
            vecSubIntDulArray.emplace_back(atoi(subValue.c_str()));
          }
          data.IntDulArray.emplace_back(vecSubIntDulArray);
        }
        std::string strBoolArray = (*iter)["BoolArray"].get<std::string>();
        std::vector<std::string> vecStrBoolArray = _Internal::split(strBoolArray, ';');
        for (auto& value : vecStrBoolArray) {
          data.BoolArray.emplace_back(value == "true");
        }
        std::string strBoolDulArray = (*iter)["BoolDulArray"].get<std::string>();
        std::vector<std::string> vecStrBoolDulArray = _Internal::split(strBoolDulArray, '|');
        for (auto& value : vecStrBoolDulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          std::vector<bool> vecSubBoolDulArray;
          for (auto& subValue : _vecSubValues) {
            vecSubBoolDulArray.emplace_back(subValue == "true");
          }
          data.BoolDulArray.emplace_back(vecSubBoolDulArray);
        }
        std::string strStringArray = (*iter)["StringArray"].get<std::string>();
        std::vector<std::string> vecStrStringArray = _Internal::split(strStringArray, ';');
        for (auto& value : vecStrStringArray) {
          data.StringArray.emplace_back(value.c_str());
        }
        std::string strStringDulArray = (*iter)["StringDulArray"].get<std::string>();
        std::vector<std::string> vecStrStringDulArray = _Internal::split(strStringDulArray, '|');
        for (auto& value : vecStrStringDulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          data.StringDulArray.emplace_back(_vecSubValues);
        }
        std::string strInt64Array = (*iter)["Int64Array"].get<std::string>();
        std::vector<std::string> vecStrInt64Array = _Internal::split(strInt64Array, ';');
        for (auto& value : vecStrInt64Array) {
          data.Int64Array.emplace_back(std::stoll(value.c_str()));
        }
        std::string strInt64DulArray = (*iter)["Int64DulArray"].get<std::string>();
        std::vector<std::string> vecStrInt64DulArray = _Internal::split(strInt64DulArray, '|');
        for (auto& value : vecStrInt64DulArray) {
          std::vector<std::string> _vecSubValues = _Internal::split(value, ';');
          std::vector<int64_t> vecSubInt64DulArray;
          for (auto& subValue : _vecSubValues) {
            vecSubInt64DulArray.emplace_back(std::stoll(subValue.c_str()));
          }
          data.Int64DulArray.emplace_back(vecSubInt64DulArray);
        }
        uint64_t key = MAKE_UINT64(data.Index, data.ConditionID);
        m_mapData[key] = data;
      }
    }
    catch (std::exception& ex) {
      return false;
    }
    return true;
  }
  void TwoIndexInst::Clear() {
    m_mapData.clear();
  }
  const TwoIndexInst::Data* TwoIndexInst::GetData(INDEX_TYPE_ nIndex, INDEX_TYPE_ nConditionID) {
    uint64_t key = MAKE_UINT64(nIndex, nConditionID);
    auto it = m_mapData.find(key);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  const TwoIndexInst::ContainerType& TwoIndexInst::GetAllData() {
    return m_mapData;
  }
  // ================== TwoIndex.csv end ==================
}
