#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

// ������� ��� ������������ ���������������� ����� ���� uint64_t � �����
std::string uint64ToHexString(uint64_t value) {
    std::stringstream ss;
   ss << std::hex << std::uppercase << std::setw(16) << std::setfill('0') << value;
    return ss.str();
}

// ������� ��� �������� ���� ������� ����� � hex-������������
std::string multiplyHexNumbers(const std::string& hexNumber1, const std::string& hexNumber2) {
    uint64_t number1 = std::stoull(hexNumber1, nullptr, 16);
    uint64_t number2 = std::stoull(hexNumber2, nullptr, 16);
    uint64_t result = number1 * number2;
    return uint64ToHexString(result);
}

int main() {
    std::string inputFile1 = "C:\\Users\\arina\\Desktop\\input1.txt";  // ���� �� ������� �������� �����
    std::string inputFile2 = "C:\\Users\\arina\\Desktop\\input2.txt";  // ���� �� ������� �������� �����
    std::string outputFile = "C:\\Users\\arina\\Desktop\\output.txt";  // ���� �� ��������� �����

    // ���������� ������� �����
    std::ifstream file1(inputFile1);
    std::ifstream file2(inputFile2);

    if (!file1 || !file2) {
        std::cerr << "error in" << std::endl;
        return 1;
    }

    std::string hexNumber1;
    std::string hexNumber2;

    std::getline(file1, hexNumber1);
    std::getline(file2, hexNumber2);

    file1.close();
    file2.close();

    // �������� �����
    std::string result = multiplyHexNumbers(hexNumber1, hexNumber2);

    // ����� ���������� � �������� ����
    std::ofstream outputFileStream(outputFile);
    if (!outputFileStream.is_open()) {
        std::cerr << "error out" << std::endl;
        return 1;
    }

    outputFileStream << result;
    outputFileStream.close();

    std::cout << "completed " << outputFile << std::endl;

    return 0;
}