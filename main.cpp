#include <iostream>

int main(int, char**) {
    std::cout << "Hello, world!\n";
}
 

int fff() {
    std::ifstream file("Activity.csv");
    aria::csv::CsvParser parser(file);
    
    for (auto& row: parser) {
        for (auto& field : row) {
            std::cout << field << ",";
        }
        std::cout << std::endl;
    }
}