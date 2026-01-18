#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <iomanip>

// 条件编译，根据不同操作系统选择睡眠函数
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class ASCIITextPlayer {
private:
    std::string file_path;
    int width;
    int height;
    int total_frames;
    std::string all_frames_data;
    int current_frame;

public:
    ASCIITextPlayer(const std::string& path) : file_path(path), width(0), height(0), total_frames(0), current_frame(0) {
        load_file();
    }

    void load_file() {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("文件 '" + file_path + "' 不存在");
        }

        std::cout << "正在加载文件: " << file_path << std::endl;

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        if (lines.size() < 3) {
            throw std::runtime_error("文件格式错误: 需要至少3行，实际只有" + std::to_string(lines.size()) + "行");
        }

        try {
            width = std::stoi(lines[0]);
            height = std::stoi(lines[1]);
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("文件格式错误: 前两行不是有效的数字");
        }

        all_frames_data = lines[2];

        int chars_per_frame = width * height;
        total_frames = all_frames_data.length() / chars_per_frame;

        std::cout << "加载完成: 尺寸 " << width << "x" << height << ", 共 " << total_frames << " 帧" << std::endl;
    }

    std::string get_frame(int frame_index) {
        if (frame_index < 0 || frame_index >= total_frames) {
            return "";
        }
        int chars_per_frame = width * height;
        int start = frame_index * chars_per_frame;
        int end = start + chars_per_frame;
        return all_frames_data.substr(start, chars_per_frame);
    }

    bool display_frame(int frame_index, int max_display_height = 25) {
        std::string frame_data = get_frame(frame_index);
        if (frame_data.empty()) {
            std::cout << "错误: 无法获取第 " << frame_index << " 帧" << std::endl;
            return false;
        }

        // 使用stringstream缓存所有输出
        std::stringstream buffer;

        // 更快的清屏方法
        #ifdef _WIN32
            // Windows: 使用特殊字符序列清屏
            buffer << "\x1B[2J\x1B[H";
        #else
            // Linux/Unix: 使用特殊字符序列清屏
            buffer << "\x1B[2J\x1B[H";
        #endif

        buffer << "=== 第 " << frame_index << "/" << total_frames - 1 << " 帧 ===" << "\n";
        buffer << "+" << std::string(width, '-') << "+" << "\n";

        for (int row = 0; row < std::min(height, max_display_height); row++) {
            int start = row * width;
            buffer << "|" << frame_data.substr(start, width) << "|" << "\n";
        }

        buffer << "+" << std::string(width, '-') << "+" << "\n";
        buffer << "命令: (n)下一帧 (p)上一帧 (g)跳转 (a)播放 (q)退出" << "\n";

        // 一次性输出所有内容
        std::cout << buffer.str();
        return true;
    }

    int play_animation(int fps = 10, int start_frame = 0) {
        if (start_frame < 0 || start_frame >= total_frames) {
            std::cout << "错误: 无效的起始帧 " << start_frame << std::endl;
            return start_frame;
        }

        std::cout << "开始播放动画 (FPS: " << fps << ")" << std::endl;
        std::cout << "按 Ctrl+C 停止播放" << std::endl;
        int frame_duration_ms = static_cast<int>((1.0 / fps) * 1000);
        int current = start_frame;

        try {
            while (current < total_frames) {
                display_frame(current, 50);
                std::cout << "播放中... 帧: " << current << "/" << total_frames - 1 << "\r";
                current++;
                // 使用操作系统特定的睡眠函数
                #ifdef _WIN32
                    Sleep(frame_duration_ms);
                #else
                    usleep(frame_duration_ms * 1000);  // usleep需要微秒
                #endif
            }
            std::cout << "\n播放完成!" << std::endl;
        } catch (const std::exception&) {
            std::cout << "\n播放已停止" << std::endl;
        }
        return current - 1;
    }

    void run() {
        // 关闭cout同步，提高输出速度
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);

        // 显示第一帧
        display_frame(0);

        // 简单交互循环
        while (true) {
            std::cout << "> ";
            std::string cmd;
            std::getline(std::cin, cmd);

            if (cmd == "n" || cmd.empty()) {
                current_frame = std::min(current_frame + 1, total_frames - 1);
                display_frame(current_frame);
            } else if (cmd == "p") {
                current_frame = std::max(current_frame - 1, 0);
                display_frame(current_frame);
            } else if (cmd.substr(0, 1) == "g") {
                try {
                    int frame_num = std::stoi(cmd.substr(1));
                    if (0 <= frame_num && frame_num < total_frames) {
                        current_frame = frame_num;
                        display_frame(current_frame);
                    } else {
                        std::cout << "帧号必须在 0-" << total_frames - 1 << " 之间" << std::endl;
                    }
                } catch (const std::invalid_argument&) {
                    std::cout << "请输入有效的帧号，例如: g 100" << std::endl;
                }
            } else if (cmd == "a") {
                std::cout << "请输入帧率 (默认 10): ";
                std::string fps_input;
                std::getline(std::cin, fps_input);
                int fps = fps_input.empty() ? 10 : std::stoi(fps_input);
                int last_frame = play_animation(fps, current_frame);
                current_frame = std::min(last_frame, total_frames - 1);
                display_frame(current_frame);
            } else if (cmd == "q") {
                std::cout << "退出预览器" << std::endl;
                break;
            } else {
                std::cout << "未知命令: " << cmd << std::endl;
                std::cout << "可用命令: n(下一帧), p(上一帧), g 数字(跳转), a(播放), q(退出)" << std::endl;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    // 关闭cout同步，提高输出速度
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc < 2) {
        std::cout << "用法: play <文本文件>" << std::endl;
        std::cout << "示例: play output.txt" << std::endl;
        return 1;
    }

    std::string file_path = argv[1];

    try {
        ASCIITextPlayer player(file_path);
        player.run();
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}