import sys
import os
import time

class ASCIITextPlayer:
    def __init__(self, file_path):
        self.file_path = file_path
        self.width = 0
        self.height = 0
        self.total_frames = 0
        self.all_frames_data = ""
        self.current_frame = 0
        self.load_file()
    
    def load_file(self):
        """加载output.txt文件"""
        if not os.path.exists(self.file_path):
            raise FileNotFoundError(f"文件 '{self.file_path}' 不存在")
        
        print(f"正在加载文件: {self.file_path}")
        
        with open(self.file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        if len(lines) < 3:
            raise ValueError(f"文件格式错误: 需要至少3行，实际只有{len(lines)}行")
        
        try:
            self.width = int(lines[0].strip())
            self.height = int(lines[1].strip())
        except ValueError:
            raise ValueError("文件格式错误: 前两行不是有效的数字")
        
        self.all_frames_data = lines[2].strip('\n')
        
        chars_per_frame = self.width * self.height
        self.total_frames = len(self.all_frames_data) // chars_per_frame
        
        print(f"加载完成: 尺寸 {self.width}x{self.height}, 共 {self.total_frames} 帧")
    
    def get_frame(self, frame_index):
        """获取指定帧的ASCII字符画"""
        if frame_index < 0 or frame_index >= self.total_frames:
            return None
        chars_per_frame = self.width * self.height
        start = frame_index * chars_per_frame
        end = start + chars_per_frame
        return self.all_frames_data[start:end]
    
    def display_frame(self, frame_index, max_display_height=25):
        """在控制台显示指定帧"""
        frame_data = self.get_frame(frame_index)
        if frame_data is None:
            print(f"错误: 无法获取第 {frame_index} 帧")
            return False
        
        # 清屏并显示
        os.system('cls' if os.name == 'nt' else 'clear')
        print(f"=== 第 {frame_index}/{self.total_frames-1} 帧 ===")
        print("+" + "-" * self.width + "+")
        
        for row in range(min(self.height, max_display_height)):
            start = row * self.width
            end = start + self.width
            print("|" + frame_data[start:end] + "|")
        
        print("+" + "-" * self.width + "+")
        print(f"命令: (n)下一帧 (p)上一帧 (g)跳转 (a)播放 (q)退出")
        return True
    
    def play_animation(self, fps=10, start_frame=0):
        """播放ASCII动画 (简化版)"""
        if start_frame < 0 or start_frame >= self.total_frames:
            print(f"错误: 无效的起始帧 {start_frame}")
            return
        
        print(f"开始播放动画 (FPS: {fps})")
        print("按 Ctrl+C 停止播放")
        frame_duration = 1.0 / fps
        current = start_frame
        
        try:
            while current < self.total_frames:
                self.display_frame(current, max_display_height=50)
                print(f"播放中... 帧: {current}/{self.total_frames-1}")
                current += 1
                time.sleep(frame_duration)
            print("播放完成!")
        except KeyboardInterrupt:
            print("\n播放已停止")
        return current - 1

def main():
    if len(sys.argv) < 2:
        print("用法: python preview_ascii_fixed.py <文本文件>")
        print("示例: python preview_ascii_fixed.py output.txt")
        return
    
    file_path = sys.argv[1]
    
    try:
        player = ASCIITextPlayer(file_path)
        
        # 显示第一帧
        player.display_frame(0)
        
        # 简单交互循环
        while True:
            cmd = input("> ").strip().lower()
            
            if cmd == 'n' or cmd == '':
                player.current_frame = min(player.current_frame + 1, player.total_frames - 1)
                player.display_frame(player.current_frame)
            elif cmd == 'p':
                player.current_frame = max(player.current_frame - 1, 0)
                player.display_frame(player.current_frame)
            elif cmd.startswith('g'):
                try:
                    parts = cmd.split()
                    if len(parts) > 1:
                        frame_num = int(parts[1])
                        if 0 <= frame_num < player.total_frames:
                            player.current_frame = frame_num
                            player.display_frame(player.current_frame)
                        else:
                            print(f"帧号必须在 0-{player.total_frames-1} 之间")
                except ValueError:
                    print("请输入有效的帧号，例如: g 100")
            elif cmd == 'a':
                fps_input = input("请输入帧率 (默认 10): ").strip()
                fps = int(fps_input) if fps_input.isdigit() else 10
                last_frame = player.play_animation(fps, player.current_frame)
                if last_frame is not None:
                    player.current_frame = min(last_frame, player.total_frames - 1)
                    player.display_frame(player.current_frame)
            elif cmd == 'q':
                print("退出预览器")
                break
            else:
                print(f"未知命令: {cmd}")
                print("可用命令: n(下一帧), p(上一帧), g 数字(跳转), a(播放), q(退出)")
    
    except Exception as e:
        print(f"错误: {e}")

if __name__ == "__main__":
    main()