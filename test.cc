#include "gtest/gtest.h"
#include "csv.hpp"
#include "CsvConfigInsts.h"

std::string csvFilePath = "";
std::string csvTwoIndexFilePath = "";
#define HEAD_ROW 0
#define HEADER_COUNT 3

TEST(LoadData, TestOneIndexDataCount) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));

    std::ifstream file(csvFile);
    bool open = file.is_open();
    EXPECT_TRUE(open);
    if (!file.is_open()) {
        return;
    } 

    int nActualNum = 0 - HEADER_COUNT;
    file.seekg(0);
    char c;
    while (!file.eof() && file.get(c)) {
        if (c == '\n') {
            ++nActualNum;
        }
    }
    EXPECT_EQ(Csv::OneIndexInst::Instance().GetAllData().size(), nActualNum);
}

TEST(LoadData, TestTwoIndexDataCount) {
    std::string csvFile = csvTwoIndexFilePath;
    EXPECT_TRUE(Csv::TwoIndexInst::Instance().Load(csvFile));

    std::ifstream file(csvFile);
    bool open = file.is_open();
    EXPECT_TRUE(open);
    if (!file.is_open()) {
        return;
    } 

    int nActualNum = 0 - HEADER_COUNT;
    file.seekg(0);
    char c;
    while (!file.eof() && file.get(c)) {
        if (c == '\n') {
            ++nActualNum;
        }
    }

    EXPECT_EQ(Csv::TwoIndexInst::Instance().GetAllData().size(), nActualNum);
}

TEST(GetData, TestOneIndexData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            EXPECT_EQ(pData->Index, (*iter)["Index"].get<int32_t>()) << "csv index is: " << csvIdx;
            EXPECT_EQ(pData->ConditionID, (*iter)["ConditionID"].get<int32_t>()) << "csv index is: " << csvIdx;
            EXPECT_EQ(pData->Type, (*iter)["Type"].get<int32_t>()) << "csv index is: " << csvIdx;
        }
    }
}

TEST(GetData, TestTwoIndexData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::TwoIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx1 = (*iter)["Index"].get<int32_t>();
        int32_t csvIdx2 = (*iter)["ConditionID"].get<int32_t>();
        auto* pData = Csv::TwoIndexInst::Instance().GetData(csvIdx1, csvIdx2);
        EXPECT_NE(pData, nullptr) << "csv index1 is: " << csvIdx1 << ", index2 is: " << csvIdx2;
        if (pData != nullptr) {
            EXPECT_EQ(pData->Index, (*iter)["Index"].get<int32_t>()) << "csv index is: " << csvIdx1 << ", index2 is: " << csvIdx2;
            EXPECT_EQ(pData->ConditionID, (*iter)["ConditionID"].get<int32_t>()) << "csv index is: " << csvIdx1 << ", index2 is: " << csvIdx2;
            EXPECT_EQ(pData->Type, (*iter)["Type"].get<int32_t>()) << "csv index is: " << csvIdx1 << ", index2 is: " << csvIdx2;
        }
    }
}

TEST(GetData, TestIntArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["IntArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, ';');
            auto& vecTargetArray = pData->IntArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                    EXPECT_EQ(vecTargetArray[arrIdx], atoi(out[arrIdx].c_str())) << "csv index is: " << csvIdx;
                }
            }
        }
    }
}

TEST(GetData, TestInt64ArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["Int64Array"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, ';');
            auto& vecTargetArray = pData->Int64Array;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                    EXPECT_EQ(vecTargetArray[arrIdx], std::stoll(out[arrIdx].c_str())) << "csv index is: " << csvIdx;
                }
            }
        }
    }
}

TEST(GetData, TestStringArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["StringArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, ';');
            auto& vecTargetArray = pData->StringArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                    EXPECT_EQ(vecTargetArray[arrIdx], out[arrIdx]) << "csv index is: " << csvIdx;
                }
            }
        }
    }
}

TEST(GetData, TestBoolArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["BoolArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, ';');
            auto& vecTargetArray = pData->BoolArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                    EXPECT_EQ(vecTargetArray[arrIdx], out[arrIdx] == "true") << "csv index is: " << csvIdx;
                }
            }
        }
    }
}

TEST(GetData, TestIntDulArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["IntDulArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, '|');
            auto& vecTargetArray = pData->IntDulArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int idx = 0; idx < out.size(); ++idx) {
                    std::vector<std::string> subOut = Csv::_Internal::split(out[idx], ';');
                    for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                        EXPECT_EQ(vecTargetArray[idx][arrIdx], atoi(subOut[arrIdx].c_str())) << "csv index is: " << csvIdx;
                    }
                }
            }
        }
    }
}

TEST(GetData, TestInt64DulArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["Int64DulArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, '|');
            auto& vecTargetArray = pData->Int64DulArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int idx = 0; idx < out.size(); ++idx) {
                    std::vector<std::string> subOut = Csv::_Internal::split(out[idx], ';');
                    for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                        EXPECT_EQ(vecTargetArray[idx][arrIdx], std::stoll(subOut[arrIdx].c_str())) << "csv index is: " << csvIdx;
                    }
                }
            }
        }
    }
}

TEST(GetData, TestStringDulArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["StringDulArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, '|');
            auto& vecTargetArray = pData->StringDulArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int idx = 0; idx < out.size(); ++idx) {
                    std::vector<std::string> subOut = Csv::_Internal::split(out[idx], ';');
                    for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                        EXPECT_EQ(vecTargetArray[idx][arrIdx], subOut[arrIdx]) << "csv index is: " << csvIdx;
                    }
                }
            }
        }
    }
}

TEST(GetData, TestBoolDulArrayData) {
    std::string csvFile = csvFilePath;
    EXPECT_TRUE(Csv::OneIndexInst::Instance().Load(csvFile));
    csv::CSVFormat format;
    format.delimiter({ ',' })
        .quote('"')
        .header_row(HEAD_ROW)
        .detect_bom(true);

    csv::CSVReader originData(csvFile, format);
    auto iter = originData.begin();
    for (int i = 0; iter != originData.end(); ++iter, ++i) {
        if (i < 3) {
            continue;
        }

        int32_t csvIdx = (*iter)["Index"].get<int32_t>();
        auto* pData = Csv::OneIndexInst::Instance().GetData(csvIdx);
        EXPECT_NE(pData, nullptr) << "csv index is: " << csvIdx;
        if (pData != nullptr) {
            std::string values = (*iter)["BoolDulArray"].get<>();
            std::vector<std::string> out = Csv::_Internal::split(values, '|');
            auto& vecTargetArray = pData->BoolDulArray;
            EXPECT_EQ(vecTargetArray.size(), out.size()) << "csv index is: " << csvIdx;
            if (vecTargetArray.size() == out.size()) {
                for (int idx = 0; idx < out.size(); ++idx) {
                    std::vector<std::string> subOut = Csv::_Internal::split(out[idx], ';');
                    for (int arrIdx = 0; arrIdx < vecTargetArray.size(); ++arrIdx) {
                        EXPECT_EQ(vecTargetArray[idx][arrIdx], subOut[arrIdx] == "true") << "csv index is: " << csvIdx;
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Arguments must be TWO." << std::endl;
        return 1;
    }

    csvFilePath = argv[1];
    csvTwoIndexFilePath = argv[2];

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

