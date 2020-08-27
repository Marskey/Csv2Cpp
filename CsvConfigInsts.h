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
      // ��ˮ̖
      Index Index;
      // �l����̖
      int32_t ConditionID;
      // �l�����
      int32_t Type;
      // �l��ֵ1
      std::vector<int32_t> Values;
      // �������
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
      // �΄վ�̖
      Index VillageQuestID;
      // �΄�Ⱥ�M
      Index QuestGroup;
      // �΄ճ�ȡ�C��
      int32_t QuestProbability;
      // �΄����Q
      std::string QuestName;
      // �΄�����
      std::string QuestInfo;
      // �΄յȼ�
      int32_t QuestLevel;
      // �΄պĕr
      int32_t QuestTime;
      // �S�C�¼�Ⱥ�M
      int32_t RandomEventGroup;
      // �S�C�¼��|�l�C��
      int32_t RandomEventProbability;
      // ����Ҫ�����
      int32_t AttributeConditionType;
      // ����Ҫ�󅢔�
      int32_t AttributeConditionValue;
      // �����@ʾ1
      std::vector<std::vector<int32_t>> Rewards;
      // ��Ҫ�����
      int32_t ReputationConditionType;
      // ��Ҫ�󅢔�
      int32_t ReputationConditionValue;
      // ���f��
      int32_t VillageReputation;
      // Ӣ�ی������1
      std::vector<std::vector<int32_t>> Attributes;
      // ��������
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
      // ��؈D���B��̖
      Index WorldBuildingID;
      // ���Q
      std::string Name;
      // �N�
      int32_t Type;
      // ���R��
      int32_t Defense;
      // ħ�������;�
      int32_t MaxMagicShieldHP;
      // �Ǡ��;�
      int32_t MaxBuildingHP;
      // �S�޳Ǡ��������
      int32_t BuildingRepairItem;
      // �S�޳Ǡ�������
      int32_t BuildingRepairItemCount;
      // �S����s�r�g(S)
      int32_t BuildingRepairCD;
      // �v������
      int32_t ReinforcementCapacity;
      // �Ƿ��������
      int32_t BuildingTroop;
      // �Ƿ���꠱���
      int32_t BuildingTroopCount;
      // �����ʯ����
      int32_t ExtinguishingGemCount;
      // ����˷e����
      int32_t ExtinguishingAllianceCreditCount;
      // �����s�r�g(S)
      int32_t ExtinguishingCD;
      // ȼ���r�g(S)
      int32_t BurningTime;
      // �I�ع���
      int32_t Area;
      // �����f��
      std::string Info;
      // ��������
      int32_t ReinforcementType;
      // �����M��ֵ
      int32_t Progress;
      // �v������
      int32_t ReinforcementTroopLimit;
      // �Ƿ�Ӣ������
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
      // ����N�
      Index WorldObjectID;
      // �a��ֱ��
      int32_t OutputDetour;
      // �ϵKֱ��
      int32_t CollisionDetour;
      // �ɷ�ɲ�
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
      // �^��ID
      Index WorldRegionID;
      // �^�����Q
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
