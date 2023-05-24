#include <sstream>
#include <iomanip>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

constexpr uint32_t BLOCK_SIZE = 32 * 1024 * 1024;  // 32 megabytes

std::string uint32ToHexString(uint32_t value) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
    return ss.str();
}

std::vector<boost::multiprecision::cpp_int> splitNumber(const std::string& hexNumber) {
    std::vector<boost::multiprecision::cpp_int> blocks;
    std::istringstream iss(hexNumber);

    std::string blockHex;
    while (iss >> std::setw(8) >> blockHex) {
        boost::multiprecision::cpp_int block(blockHex, 16);
        blocks.push_back(block);
    }

    return blocks;
}

std::string mergeBlocks(const std::vector<boost::multiprecision::cpp_int>& blocks) {
    std::stringstream ss;
    for (const boost::multiprecision::cpp_int& block : blocks) {
        ss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << block;
    }
    return ss.str();
}

std::vector<boost::multiprecision::cpp_int> addNumbers(const std::vector<boost::multiprecision::cpp_int>& number1,
    const std::vector<boost::multiprecision::cpp_int>& number2) {
    int n = std::max(number1.size(), number2.size());

    std::vector<boost::multiprecision::cpp_int> result(n);

    boost::multiprecision::cpp_int carry = 0;

    for (int i = 0; i < n; i++) {
        boost::multiprecision::cpp_int sum = carry;
        if (i < number1.size()) {
            sum += number1[i];
        }
        if (i < number2.size()) {
            sum += number2[i];
        }

        result[i] = sum;
        carry = sum >> (BLOCK_SIZE * 8);
    }

    if (carry != 0) {
        result.push_back(carry);
    }

    return result;
}

std::vector<boost::multiprecision::cpp_int> karatsubaMultiply(const std::vector<boost::multiprecision::cpp_int>& number1,
    const std::vector<boost::multiprecision::cpp_int>& number2) {
    int n = number1.size();

    if (n <= 32) {
        return multiplyNumbers(number1, number2);
    }

    int half = n / 2;

    std::vector<boost::multiprecision::cpp_int> high1(number1.begin(), number1.begin() + half);
    std::vector<boost::multiprecision::cpp_int> low1(number1.begin() + half, number1.end());

    std::vector<boost::multiprecision::cpp_int> high2(number2.begin(), number2.begin() + half);
    std::vector<boost::multiprecision::cpp_int> low2(number2.begin() + half, number2.end());

    std::vector<boost::multiprecision::cpp_int> z0 = karatsubaMultiply(low1, low2);
    std::vector<boost::multiprecision::cpp_int> z2 = karatsubaMultiply(high1, high2);

    std::vector<boost::multiprecision::cpp_int> sum1 = addNumbers(high1, low1);
    std::vector<boost::multiprecision::cpp_int> sum2 = addNumbers(high2, low2);

    std::vector<boost::multiprecision::cpp_int> z1 = karatsubaMultiply(sum1, sum2);

    std::vector<boost::multiprecision::cpp_int> subResult1 = addNumbers(z2, z0);
    std::vector<boost::multiprecision::cpp_int> subResult2 = addNumbers(z1, subResult1);

    std::vector<boost::multiprecision::cpp_int> result(2 * n);

    for (int i = 0; i < z0.size(); i++) {
        result[i] = z0[i];
    }

    for (int i = 0; i < subResult1.size(); i++) {
        result[i + half] += subResult1[i];
    }

    for (int i = 0; i < subResult2.size(); i++) {
        result[i + n] += subResult2[i];
    }

    return result;
}

std::vector<boost::multiprecision::cpp_int> multiplyNumbers(const std::vector<boost::multiprecision::cpp_int>& number1,
    const std::vector<boost::multiprecision::cpp_int>& number2) {
    int n = number1.size();

    if (n == 1) {
        std::vector<boost::multiprecision::cpp_int> result(2);
        result[0] = number1[0] * number2[0];
        result[1] = result[0] >> (BLOCK_SIZE * 8);
        result[0] &= (1ULL << (BLOCK_SIZE * 8)) - 1;
        return result;
    }

    int half = n / 2;
    std::vector<boost::multiprecision::cpp_int> high1(number1.begin(), number1.begin() + half);
    std::vector<boost::multiprecision::cpp_int> low1(number1.begin() + half, number1.end());

    std::vector<boost::multiprecision::cpp_int> high2(number2.begin(), number2.begin() + half);
    std::vector<boost::multiprecision::cpp_int> low2(number2.begin() + half, number2.end());

    std::vector<boost::multiprecision::cpp_int> z0 = multiplyNumbers(low1, low2);
    std::vector<boost::multiprecision::cpp_int> z2 = multiplyNumbers(high1, high2);

    std::vector<boost::multiprecision::cpp_int> sum1 = addNumbers(high1, low1);
    std::vector<boost::multiprecision::cpp_int> sum2 = addNumbers(high2, low2);

    std::vector<boost::multiprecision::cpp_int> z1 = multiplyNumbers(sum1, sum2);

    std::vector<boost::multiprecision::cpp_int> subResult1 = addNumbers(z2, z0);
    std::vector<boost::multiprecision::cpp_int> subResult2 = addNumbers(z1, subResult1);

    std::vector<boost::multiprecision::cpp_int> result(2 * n);

    for (int i = 0; i < z0.size(); i++) {
        result[i] = z0[i];
    }

    for (int i = 0; i < subResult1.size(); i++) {
        result[i + half] += subResult1[i];
    }

    for (int i = 0; i < subResult2.size(); i++) {
        result[i + n] += subResult2[i];
    }

    return result;
}

std::string multiplyHexNumbers(const std::string& hexNumber1, const std::string& hexNumber2) {
    std::vector<boost::multiprecision::cpp_int> number1 = splitNumber(hexNumber1);
    std::vector<boost::multiprecision::cpp_int> number2 = splitNumber(hexNumber2);

    std::vector<boost::multiprecision::cpp_int> result = karatsubaMultiply(number1, number2);

    return mergeBlocks(result);
}

int main() {
    std::string inputFile1 = "input1.txt";  // Шлях до першого вхідного файлу
    std::string inputFile2 = "input2.txt";  // Шлях до другого вхідного файлу
    std::string outputFile = "output.txt";  // Шлях до вихідного файлу

    std::ifstream file1(inputFile1);
    std::ifstream file2(inputFile2);

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "Error in" << std::endl;
        return 1;
    }

    std::string hexNumber1;
    std::string hexNumber2;

    std::getline(file1, hexNumber1);
    std::getline(file2, hexNumber2);

    file1.close();
    file2.close();

    std::string result = multiplyHexNumbers(hexNumber1, hexNumber2);

    std::ofstream outputFileStream(outputFile);
    if (!outputFileStream.is_open()) {
        std::cerr << "Error out" << std::endl;
        return 1;
    }

    outputFileStream << result;
    outputFileStream.close();

    std::cout << "Success , result save in:" << outputFile << std::endl;

    return 0;
}