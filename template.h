#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
namespace Csv {
  #define MAKE_UINT64(a, b) (((uint64_t)(a) & 0xffffffffUL) | ((((uint64_t)(b)) & 0xffffffffUL) << 32))
  #define CSV_DECLARE_SINGLETON(__class_name__)\
  public:\
    static __class_name__& Instance() {\
      static __class_name__ instance;\
      return instance;\
    }\
  private:\
    __class_name__() = default;\
    ~__class_name__() = default;\
    __class_name__(const __class_name__&) = default;\
    __class_name__& operator=(const __class_name__&) = default;\
  using Index = int32_t;
  // ================== Condition.csv start ==================

  class CConditionConfig
  {
    CSV_DECLARE_SINGLETON(CConditionConfig)
  public:
    struct ConditionData
    {
      // 流水號
      Index Index;
      // 條件編號
      int32_t ConditionID;
      // 條件類型
      int32_t Type;
      // 條件值1
      std::vector<int32_t> Values;
      // 索引類型
      std::vector<std::vector<int32_t>> TableType;
    };
  public:
    bool Load(const std::string& filePath);
    bool Clear();
    const ConditionData* GetData(Index nIndex);
  private:
    std::unordered_map<uint64_t, ConditionData> m_mapData;
  };
  // ================== Condition.csv end ==================

  static std::vector<std::string> split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
      elems.emplace_back(item);
    }
    return elems;
  }
};
