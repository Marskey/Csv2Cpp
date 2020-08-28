#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>
namespace Csv {
  #define MAKE_UINT64(hi, lo) ((uint64_t)(((uint32_t)(lo) & 0xffffffffUL) | ((uint64_t)(((uint32_t)(hi)) & 0xffffffffUL)) << 32))
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

  using INDEX_TYPE_ = int32_t;

  // ================== OneIndex.csv start ==================
  class OneIndexInst
  {
    CSV_DECLARE_SINGLETON(OneIndexInst)
  public:
    struct Data
    {
      // 流水號
      INDEX_TYPE_ Index;
      // 條件編號
      int32_t ConditionID;
      // 條件類型
      int32_t Type;
      // 條件值1
      std::vector<int32_t> IntArray;
      // 索引類型
      std::vector<std::vector<int32_t>> IntDulArray;
      // 布尔数组
      std::vector<bool> BoolArray;
      // 布尔二维数组
      std::vector<std::vector<bool>> BoolDulArray;
      // 字符数组
      std::vector<std::string> StringArray;
      // 字符二维数组
      std::vector<std::vector<std::string>> StringDulArray;
      // 64位整型数组
      std::vector<int64_t> Int64Array;
      // 64位整型二维数组
      std::vector<std::vector<int64_t>> Int64DulArray;
    };
    using ContainerType = std::map<uint64_t, Data>;
  public:
    bool Load(const std::string& filePath);
    void Clear();
    const Data* GetData(INDEX_TYPE_ nIndex);
    const ContainerType& GetAllData();
  private:
    ContainerType m_mapData;
  };
  // ================== OneIndex.csv end ==================

  // ================== TwoIndex.csv start ==================
  class TwoIndexInst
  {
    CSV_DECLARE_SINGLETON(TwoIndexInst)
  public:
    struct Data
    {
      // 流水號
      INDEX_TYPE_ Index;
      // 條件編號
      INDEX_TYPE_ ConditionID;
      // 條件類型
      int32_t Type;
      // 條件值1
      std::vector<int32_t> IntArray;
      // 索引類型
      std::vector<std::vector<int32_t>> IntDulArray;
      // 布尔数组
      std::vector<bool> BoolArray;
      // 布尔二维数组
      std::vector<std::vector<bool>> BoolDulArray;
      // 字符数组
      std::vector<std::string> StringArray;
      // 字符二维数组
      std::vector<std::vector<std::string>> StringDulArray;
      // 64位整型数组
      std::vector<int64_t> Int64Array;
      // 64位整型二维数组
      std::vector<std::vector<int64_t>> Int64DulArray;
    };
    using ContainerType = std::map<uint64_t, Data>;
  public:
    bool Load(const std::string& filePath);
    void Clear();
    const Data* GetData(INDEX_TYPE_ nIndex, INDEX_TYPE_ nConditionID);
    const ContainerType& GetAllData();
  private:
    ContainerType m_mapData;
  };
  // ================== TwoIndex.csv end ==================
  namespace _Internal {
    static std::vector<std::string> split(const std::string& s, const char delim) {
      std::stringstream ss(s);
      std::string item;
      std::vector<std::string> elems;
      while (std::getline(ss, item, delim)) {
        elems.emplace_back(item);
      }
      return elems;
    }
  }
};
