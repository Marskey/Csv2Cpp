#pragma once
#include <string>
#include <vector>
#include <unordered_map>
namespace Csv {
  using Index = int32_t;
  class CConditionConfig
  {
  public:
    CConditionConfig() = default;
    ~CConditionConfig() = default;
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
      int32_t TableType;
    };
  public:
    bool Load(const char* pFileName);
    bool Reload(const char* pFileName);
    void Clear();
    const ConditionData& GetData(int32_t nIndex);
  private:
    std::unordered_map<int64_t, ConditionData> m_mapData;
  };
};
