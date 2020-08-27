#include "template.h"
#include "csv.hpp"
namespace Csv {

  // ================== Condition.csv start ==================
  bool ConditionInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    csv::CSVReader csvReader(filePath, format);

    auto iter = csvReader.begin();
    for (int i = 0; iter != csvReader.end(); ++iter, ++i) {
      if (i < 3) {
        continue;
      }
      Data data;
      data.Index = (*iter)["Index"].get<Index>();
      data.ConditionID = (*iter)["ConditionID"].get<int32_t>();
      data.Type = (*iter)["Type"].get<int32_t>();
      std::string strValues = (*iter)["Values"].get<std::string>();
      std::vector<std::string> vecStrValues = _Internal::split(strValues, ';');
      for (auto& value : vecStrValues) {
        data.Values.emplace_back(atoi(value.c_str()));
      }
      std::string strTableType = (*iter)["TableType"].get<std::string>();
      std::vector<std::string> vecStrTableType = _Internal::split(strTableType, '|');
      for (auto& value : vecStrTableType) {
        std::vector<std::string> vecSubValues = _Internal::split(value, ';');
        for (auto& subValue : vecSubValues) {
          data.TableType.emplace_back(atoi(subValue.c_str()));
        }
      }
      m_mapData[data.Index] = data;
    }
    return true;
  }
  void ConditionInst::Clear() {
    m_mapData.clear();
  }
  const ConditionInst::Data* ConditionInst::GetData(Index nIndex) {
    auto it = m_mapData.find(nIndex);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  // ================== Condition.csv end ==================

  // ================== VillageQuest.csv start ==================
  bool VillageQuestInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    csv::CSVReader csvReader(filePath, format);

    auto iter = csvReader.begin();
    for (int i = 0; iter != csvReader.end(); ++iter, ++i) {
      if (i < 3) {
        continue;
      }
      Data data;
      data.VillageQuestID = (*iter)["VillageQuestID"].get<Index>();
      data.QuestGroup = (*iter)["QuestGroup"].get<Index>();
      data.QuestProbability = (*iter)["QuestProbability"].get<int32_t>();
      data.QuestName = (*iter)["QuestName"].get<std::string>();
      data.QuestInfo = (*iter)["QuestInfo"].get<std::string>();
      data.QuestLevel = (*iter)["QuestLevel"].get<int32_t>();
      data.QuestTime = (*iter)["QuestTime"].get<int32_t>();
      data.RandomEventGroup = (*iter)["RandomEventGroup"].get<int32_t>();
      data.RandomEventProbability = (*iter)["RandomEventProbability"].get<int32_t>();
      data.AttributeConditionType = (*iter)["AttributeConditionType"].get<int32_t>();
      data.AttributeConditionValue = (*iter)["AttributeConditionValue"].get<int32_t>();
      std::string strRewards = (*iter)["Rewards"].get<std::string>();
      std::vector<std::string> vecStrRewards = _Internal::split(strRewards, '|');
      for (auto& value : vecStrRewards) {
        std::vector<std::string> vecSubValues = _Internal::split(value, ';');
        for (auto& subValue : vecSubValues) {
          data.Rewards.emplace_back(atoi(subValue.c_str()));
        }
      }
      data.ReputationConditionType = (*iter)["ReputationConditionType"].get<int32_t>();
      data.ReputationConditionValue = (*iter)["ReputationConditionValue"].get<int32_t>();
      data.VillageReputation = (*iter)["VillageReputation"].get<int32_t>();
      std::string strAttributes = (*iter)["Attributes"].get<std::string>();
      std::vector<std::string> vecStrAttributes = _Internal::split(strAttributes, '|');
      for (auto& value : vecStrAttributes) {
        std::vector<std::string> vecSubValues = _Internal::split(value, ';');
        for (auto& subValue : vecSubValues) {
          data.Attributes.emplace_back(atoi(subValue.c_str()));
        }
      }
      data.VillageQuestReward = (*iter)["VillageQuestReward"].get<int32_t>();
      uint16_t key = MAKE_UINT64(data.VillageQuestID, data.QuestGroup);
      m_mapData[key] = data;
    }
    return true;
  }
  void VillageQuestInst::Clear() {
    m_mapData.clear();
  }
  const VillageQuestInst::Data* VillageQuestInst::GetData(Index nVillageQuestID, Index nQuestGroup) {
    uint16_t key = MAKE_UINT64(nVillageQuestID, nQuestGroup);
    auto it = m_mapData.find(key);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  // ================== VillageQuest.csv end ==================

  // ================== WorldBuilding.csv start ==================
  bool WorldBuildingInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    csv::CSVReader csvReader(filePath, format);

    auto iter = csvReader.begin();
    for (int i = 0; iter != csvReader.end(); ++iter, ++i) {
      if (i < 3) {
        continue;
      }
      Data data;
      data.WorldBuildingID = (*iter)["WorldBuildingID"].get<Index>();
      data.Name = (*iter)["Name"].get<std::string>();
      data.Type = (*iter)["Type"].get<int32_t>();
      data.Defense = (*iter)["Defense"].get<int32_t>();
      data.MaxMagicShieldHP = (*iter)["MaxMagicShieldHP"].get<int32_t>();
      data.MaxBuildingHP = (*iter)["MaxBuildingHP"].get<int32_t>();
      data.BuildingRepairItem = (*iter)["BuildingRepairItem"].get<int32_t>();
      data.BuildingRepairItemCount = (*iter)["BuildingRepairItemCount"].get<int32_t>();
      data.BuildingRepairCD = (*iter)["BuildingRepairCD"].get<int32_t>();
      data.ReinforcementCapacity = (*iter)["ReinforcementCapacity"].get<int32_t>();
      data.BuildingTroop = (*iter)["BuildingTroop"].get<int32_t>();
      data.BuildingTroopCount = (*iter)["BuildingTroopCount"].get<int32_t>();
      data.ExtinguishingGemCount = (*iter)["ExtinguishingGemCount"].get<int32_t>();
      data.ExtinguishingAllianceCreditCount = (*iter)["ExtinguishingAllianceCreditCount"].get<int32_t>();
      data.ExtinguishingCD = (*iter)["ExtinguishingCD"].get<int32_t>();
      data.BurningTime = (*iter)["BurningTime"].get<int32_t>();
      data.Area = (*iter)["Area"].get<int32_t>();
      data.Info = (*iter)["Info"].get<std::string>();
      data.ReinforcementType = (*iter)["ReinforcementType"].get<int32_t>();
      data.Progress = (*iter)["Progress"].get<int32_t>();
      data.ReinforcementTroopLimit = (*iter)["ReinforcementTroopLimit"].get<int32_t>();
      data.BuildingHero = (*iter)["BuildingHero"].get<int32_t>();
      m_mapData[data.WorldBuildingID] = data;
    }
    return true;
  }
  void WorldBuildingInst::Clear() {
    m_mapData.clear();
  }
  const WorldBuildingInst::Data* WorldBuildingInst::GetData(Index nWorldBuildingID) {
    auto it = m_mapData.find(nWorldBuildingID);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  // ================== WorldBuilding.csv end ==================

  // ================== WorldObject.csv start ==================
  bool WorldObjectInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    csv::CSVReader csvReader(filePath, format);

    auto iter = csvReader.begin();
    for (int i = 0; iter != csvReader.end(); ++iter, ++i) {
      if (i < 3) {
        continue;
      }
      Data data;
      data.WorldObjectID = (*iter)["WorldObjectID"].get<Index>();
      data.OutputDetour = (*iter)["OutputDetour"].get<int32_t>();
      data.CollisionDetour = (*iter)["CollisionDetour"].get<int32_t>();
      data.Scout = (*iter)["Scout"].get<int32_t>();
      m_mapData[data.WorldObjectID] = data;
    }
    return true;
  }
  void WorldObjectInst::Clear() {
    m_mapData.clear();
  }
  const WorldObjectInst::Data* WorldObjectInst::GetData(Index nWorldObjectID) {
    auto it = m_mapData.find(nWorldObjectID);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  // ================== WorldObject.csv end ==================

  // ================== WorldRegion.csv start ==================
  bool WorldRegionInst::Load(const std::string& filePath) {
    this->Clear();
    csv::CSVFormat format;
    format.delimiter({ ',' }).quote('"').header_row(0).detect_bom(true);
    csv::CSVReader csvReader(filePath, format);

    auto iter = csvReader.begin();
    for (int i = 0; iter != csvReader.end(); ++iter, ++i) {
      if (i < 3) {
        continue;
      }
      Data data;
      data.WorldRegionID = (*iter)["WorldRegionID"].get<Index>();
      data.Name = (*iter)["Name"].get<std::string>();
      m_mapData[data.WorldRegionID] = data;
    }
    return true;
  }
  void WorldRegionInst::Clear() {
    m_mapData.clear();
  }
  const WorldRegionInst::Data* WorldRegionInst::GetData(Index nWorldRegionID) {
    auto it = m_mapData.find(nWorldRegionID);
    if (it != m_mapData.end()) {
      return &it->second;
    }
    return nullptr;
  }
  // ================== WorldRegion.csv end ==================
}
