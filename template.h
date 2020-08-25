#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
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
      std::vector<std::vector<int32_t>> TableType;
    };
  public:
    bool Load(const char* pFileName);
    bool Reload(const char* pFileName);
    bool Clear();
    const ConditionData& GetData(int32_t nIndex);
  private:
    bool m_bInit = false;
    std::unordered_map<int64_t, ConditionData> m_mapData;
  };
  static std::vector<std::string> split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.emplace_back(item);
    }
    return elems;
  }

  class CCsvConfigMgr
  {
  public:
    CCsvConfigMgr(const CCsvConfigMgr&) = delete;
    CCsvConfigMgr& operator=(const CCsvConfigMgr&) = delete;

    static CCsvConfigMgr& instance() {
        static CCsvConfigMgr instance;
        return instance;
    }

    bool Init();
    const std::string& GetResPath();
  private:
    CCsvConfigMgr() = default;
    ~CCsvConfigMgr() = default;
  private:
    std::string m_resPath;
    CConditionConfig m_conditionConfig;
  };
};
