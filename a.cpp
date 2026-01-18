#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

// 使用改进的RLE算法压缩文本，确保输出纯文本格式
std::string compressText(const std::string& text) {
    std::stringstream compressed;
    int length = text.length();
    
    if (length == 0) {
        return "";
    }
    
    char currentChar = text[0];
    int count = 1;
    
    // 处理文本，使用@作为控制字符标记重复序列
    for (int i = 1; i < length; ++i) {
        if (text[i] == currentChar) {
            ++count;
        } else {
            // 处理当前字符序列
            if (count >= 3) {  // 只有重复次数大于等于3才使用压缩格式
                // 格式：@<字符><重复次数>
                compressed << '@' << currentChar << count;
            } else {
                // 重复次数少，直接输出字符
                compressed << std::string(count, currentChar);
            }
            // 更新当前字符和计数
            currentChar = text[i];
            count = 1;
        }
    }
    
    // 处理最后一个字符序列
    if (count >= 3) {
        compressed << '@' << currentChar << count;
    } else {
        compressed << std::string(count, currentChar);
    }
    
    return compressed.str();
}

// 分析文本并找出最频繁出现的模式
std::map<std::string, int> findFrequentPatterns(const std::string& text, int minPatternLength = 3, int minFrequency = 0) {
    std::map<std::string, int> patternCounts;
    
    // 扫描所有可能的模式
    for (int len = minPatternLength; len <= 30; ++len) {  // 检查3到30个字符的模式
        for (int i = 0; i <= (int)text.length() - len; ++i) {
            std::string pattern = text.substr(i, len);
            patternCounts[pattern]++;
        }
    }
    
    // 过滤掉出现次数太少的模式
    std::map<std::string, int> filteredPatterns;
    for (const auto& pair : patternCounts) {
        if (pair.second >= minFrequency) {
            filteredPatterns[pair.first] = pair.second;  // 修复：使用pair.first作为键
        }
    }
    
    return filteredPatterns;
}

// 根据模式的频率和长度对模式进行排序
std::vector<std::pair<std::string, int>> sortPatterns(const std::map<std::string, int>& patterns) {
    std::vector<std::pair<std::string, int>> sortedPatterns(patterns.begin(), patterns.end());
    
    // 排序规则：首先按节省的空间排序（长度*频率 - 长度 - 2），然后按频率排序
    std::sort(sortedPatterns.begin(), sortedPatterns.end(), [](const auto& a, const auto& b) {
        int savingsA = (a.first.length() * a.second) - (a.first.length() + 2);
        int savingsB = (b.first.length() * b.second) - (b.first.length() + 2);
        if (savingsA != savingsB) {
            return savingsA > savingsB;
        }
        return a.second > b.second;
    });
    
    return sortedPatterns;
}

// 使用字典压缩进一步优化RLE压缩后的文本
std::string applyDictionaryCompression(const std::string& rleCompressed, const std::vector<std::string>& patterns, std::string& dictionary) {
    std::string compressed = rleCompressed;
    dictionary = "";
    
    // 为每个模式分配一个标记
    for (int i = 0; i < (int)patterns.size() && i < 26; ++i) {  // 最多使用26个字母
        const std::string& pattern = patterns[i];
        char code = 'a' + i;
        std::string marker = "&" + std::string(1, code);
        
        // 替换所有出现的模式
        size_t pos = 0;
        while ((pos = compressed.find(pattern, pos)) != std::string::npos) {
            compressed.replace(pos, pattern.length(), marker);
            pos += marker.length();
        }
        
        // 添加到字典
        dictionary += code;
        dictionary += pattern;
        dictionary += ";";
    }
    
    return compressed;
}

int main() {
    std::string inputFile = "outputp2.txt";
    std::string outputFile = "compressed_output_v3.txt";
    
    // 打开输入文件
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "无法打开输入文件: " << inputFile << std::endl;
        return 1;
    }
    
    // 读取文件内容（保留前两行的宽度和高度）
    std::string width, height, content;
    std::getline(inFile, width);
    std::getline(inFile, height);
    std::getline(inFile, content);
    inFile.close();
    
    std::cout << "原始文件信息:" << std::endl;
    std::cout << "宽度: " << width << std::endl;
    std::cout << "高度: " << height << std::endl;
    std::cout << "内容长度: " << content.length() << " 字符" << std::endl;
    std::cout << "总大小: " << (width.length() + height.length() + content.length() + 2) << " 字节" << std::endl;
    
    // 第一步：RLE压缩
    std::string rleCompressed = compressText(content);
    
    std::cout << "\nRLE压缩结果:" << std::endl;
    std::cout << "RLE压缩后长度: " << rleCompressed.length() << " 字符" << std::endl;
    std::cout << "RLE压缩率: " << (1.0 - static_cast<double>(rleCompressed.length()) / content.length()) * 100 << "%" << std::endl;
    
    // 第二步：分析频繁模式
    auto patterns = findFrequentPatterns(rleCompressed);
    auto sortedPatterns = sortPatterns(patterns);
    
    // 提取前50个最有价值的模式
    std::vector<std::string> topPatterns;
    for (int i = 0; i < (int)sortedPatterns.size() && i < 50; ++i) {
        topPatterns.push_back(sortedPatterns[i].first);
    }
    
    // 第三步：应用字典压缩
    std::string dictionary;
    std::string finalCompressed = applyDictionaryCompression(rleCompressed, topPatterns, dictionary);
    
    std::cout << "\n字典压缩结果:" << std::endl;
    std::cout << "字典长度: " << dictionary.length() << " 字符" << std::endl;
    std::cout << "最终压缩后长度: " << finalCompressed.length() << " 字符" << std::endl;
    
    // 打开输出文件并写入压缩后的数据
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "无法创建输出文件: " << outputFile << std::endl;
        return 1;
    }
    
    // 写入宽度、高度、字典和压缩内容
    outFile << width << std::endl;
    outFile << height << std::endl;
    outFile << dictionary << std::endl;
    outFile << finalCompressed;
    outFile.close();
    
    // 计算总压缩率
    int originalSize = width.length() + height.length() + content.length() + 2;
    int compressedSize = width.length() + height.length() + dictionary.length() + finalCompressed.length() + 3;  // +3 for newlines
    
    std::cout << "\n最终压缩结果:" << std::endl;
    std::cout << "原始大小: " << originalSize << " 字节" << std::endl;
    std::cout << "压缩后大小: " << compressedSize << " 字节" << std::endl;
    std::cout << "总压缩率: " << (1.0 - static_cast<double>(compressedSize) / originalSize) * 100 << "%" << std::endl;
    std::cout << "\n压缩完成! 结果保存到 " << outputFile << std::endl;
    
    return 0;
}