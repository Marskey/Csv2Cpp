#include <gtest/gtest.h>
#include "csv.hpp"
#include "CsvConfigInsts.h"

TEST(GetData, TestGetData) {
    std::string csvFile = "Condiftion.csv";
    EXPECT_TRUE(Csv::ConditionInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(0)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        auto* pData = Csv::ConditionInst::Instance().GetData(i - 3);
        EXPECT_NE(pData, nullptr);
        if (pData != nullptr) {
            EXPECT_EQ(pData->Index, (*iter)["Index"].get<int32_t>());
            EXPECT_EQ(pData->ConditionID, (*iter)["ConditionID"].get<int32_t>());
            EXPECT_EQ(pData->Type, (*iter)["Type"].get<int32_t>());

            {
                std::string values = (*iter)["Values"].get<>();
                std::vector<std::string> out = Csv::_Internal::split(values, ';');
                EXPECT_EQ(pData->Values.size(), out.size());
                if (pData->Values.size() == out.size()) {
                    EXPECT_EQ(pData->Values[0], atoi(out[0].c_str()));
                    EXPECT_EQ(pData->Values[1], atoi(out[1].c_str()));
                }
            }
            
            {
                std::string values = (*iter)["TableType"].get<>();
                std::vector<std::string> out = Csv::_Internal::split(values, '|');
                EXPECT_EQ(pData->TableType.size(), out.size());
                if (pData->TableType.size() == out.size()) {
                    for (int idx = 0; idx < out.size(); ++idx) {
                        std::vector<std::string> subOut = Csv::_Internal::split(out[idx], ';');
                        EXPECT_EQ(pData->TableType[idx][0], atoi(subOut[0].c_str()));
                        EXPECT_EQ(pData->TableType[idx][1], atoi(subOut[1].c_str()));
                        EXPECT_EQ(pData->TableType[idx][2], atoi(subOut[2].c_str()));
                    }
                }
            }
        }
    }
    

}
