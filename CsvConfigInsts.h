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
  class ConditionInst
  {
    CSV_DECLARE_SINGLETON(ConditionInst)
  public:
    struct Data
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
    void Clear();
    const Data* GetData(Index nIndex);
  private:
    std::unordered_map<uint64_t, Data> m_mapData;
  };
  // ================== Condition.csv end ==================

  // ================== VillageQuest.csv start ==================
  class VillageQuestInst
  {
    CSV_DECLARE_SINGLETON(VillageQuestInst)
  public:
    struct Data
    {
      // 任務編號
      Index VillageQuestID;
      // 任務群組
      Index QuestGroup;
      // 任務抽取機率
      int32_t QuestProbability;
      // 任務名稱
      std::string QuestName;
      // 任務描述
      std::string QuestInfo;
      // 任務等級
      int32_t QuestLevel;
      // 任務耗時
      int32_t QuestTime;
      // 隨機事件群組
      int32_t RandomEventGroup;
      // 隨機事件觸發機率
      int32_t RandomEventProbability;
      // 屬性要求類型
      int32_t AttributeConditionType;
      // 屬性要求參數
      int32_t AttributeConditionValue;
      // 獎勵顯示1
      std::vector<std::vector<int32_t>> Rewards;
      // 聲望要求類型
      int32_t ReputationConditionType;
      // 聲望要求參數
      int32_t ReputationConditionValue;
      // 村莊聲望
      int32_t VillageReputation;
      // 英雄屬性類型1
      std::vector<std::vector<int32_t>> Attributes;
      // 獎勵索引
      int32_t VillageQuestReward;
    };
  public:
    bool Load(const std::string& filePath);
    void Clear();
    const Data* GetData(int32_t nVillageQuestID, int32_t nQuestGroup);
  private:
    std::unordered_map<uint64_t, Data> m_mapData;
  };
  // ================== VillageQuest.csv end ==================

  // ================== WorldBuilding.csv start ==================
  class WorldBuildingInst
  {
    CSV_DECLARE_SINGLETON(WorldBuildingInst)
  public:
    struct Data
    {
      // 大地圖建築編號
      Index WorldBuildingID;
      // 名稱
      std::string Name;
      // 種類
      int32_t Type;
      // 防禦力
      int32_t Defense;
      // 魔法屏障耐久
      int32_t MaxMagicShieldHP;
      // 城牆耐久
      int32_t MaxBuildingHP;
      // 維修城牆道具類型
      int32_t BuildingRepairItem;
      // 維修城牆道具量
      int32_t BuildingRepairItemCount;
      // 維修冷卻時間(S)
      int32_t BuildingRepairCD;
      // 駐防容量
      int32_t ReinforcementCapacity;
      // 城防部隊索引
      int32_t BuildingTroop;
      // 城防部隊兵量
      int32_t BuildingTroopCount;
      // 滅火寶石數量
      int32_t ExtinguishingGemCount;
      // 滅火聯盟積分量
      int32_t ExtinguishingAllianceCreditCount;
      // 滅火冷卻時間(S)
      int32_t ExtinguishingCD;
      // 燃燒時間(S)
      int32_t BurningTime;
      // 領地範圍
      int32_t Area;
      // 簡易說明
      std::string Info;
      // 建造容量
      int32_t ReinforcementType;
      // 建造進度值
      int32_t Progress;
      // 駐防限制
      int32_t ReinforcementTroopLimit;
      // 城防英雄索引
      int32_t BuildingHero;
    };
  public:
    bool Load(const std::string& filePath);
    void Clear();
    const Data* GetData(Index nWorldBuildingID);
  private:
    std::unordered_map<uint64_t, Data> m_mapData;
  };
  // ================== WorldBuilding.csv end ==================

  // ================== WorldObject.csv start ==================
  class WorldObjectInst
  {
    CSV_DECLARE_SINGLETON(WorldObjectInst)
  public:
    struct Data
    {
      // 物件種類
      Index WorldObjectID;
      // 產出直徑
      int32_t OutputDetour;
      // 障礙直徑
      int32_t CollisionDetour;
      // 可否偵查
      int32_t Scout;
    };
  public:
    bool Load(const std::string& filePath);
    void Clear();
    const Data* GetData(Index nWorldObjectID);
  private:
    std::unordered_map<uint64_t, Data> m_mapData;
  };
  // ================== WorldObject.csv end ==================

  // ================== WorldRegion.csv start ==================
  class WorldRegionInst
  {
    CSV_DECLARE_SINGLETON(WorldRegionInst)
  public:
    struct Data
    {
      // 區域ID
      Index WorldRegionID;
      // 區域名稱
      std::string Name;
    };
  public:
    bool Load(const std::string& filePath);
    void Clear();
    const Data* GetData(Index nWorldRegionID);
  private:
    std::unordered_map<uint64_t, Data> m_mapData;
  };
  // ================== WorldRegion.csv end ==================
  namespace _Internal {
    static std::vector<std::string> split(const std::string& s, char delim) {
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
