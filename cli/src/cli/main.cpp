#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> map{};
    std::vector<int> values = {13, 19, 7};
    std::unordered_map<int, std::vector<int>> tmp{};
    tmp[3] = values;
    tmp[2] = values;
    map[1] = std::move(tmp);

    std::cout << map[1][2][2] << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
