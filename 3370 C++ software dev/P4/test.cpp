#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    std::vector<int> mwa;
    std::vector<int> dataset = {1, 2, 3, 4, 5, 6};
    mwa.resize(dataset.size());
    std::copy(dataset.begin(), dataset.begin()+3, mwa.begin());
    for (auto i : mwa)
        std::cout << i << std::endl;

    return 0;
}