import cv2
import numpy as np
import os
import sys
from pathlib import Path

def images_to_ascii(frames_dir, output_path, threshold=127):
    """
    将图片序列转换为ASCII文本格式
    
    参数:
        frames_dir: 图片序列目录
        output_path: 输出文本文件路径
        threshold: 亮度阈值 (0-255)
    """
    # 获取所有图片文件并按名称排序
    frame_files = sorted([f for f in os.listdir(frames_dir) if f.lower().endswith(('.jpg', '.jpeg', '.png'))])
    
    if not frame_files:
        print(f"错误: 在目录 '{frames_dir}' 中未找到图片文件")
        return False
    
    print(f"找到 {len(frame_files)} 张图片")
    
    # 读取第一张图片获取尺寸
    first_img = cv2.imread(os.path.join(frames_dir, frame_files[0]))
    if first_img is None:
        print(f"错误: 无法读取图片 '{frame_files[0]}'")
        return False
    
    height, width = first_img.shape[:2]
    print(f"图片尺寸: {width}x{height} 像素")
    
    # 开始处理所有图片
    print("开始处理图片序列...")
    all_frames_text = ""
    processed_count = 0
    
    for frame_file in frame_files:
        img_path = os.path.join(frames_dir, frame_file)
        frame = cv2.imread(img_path)
        
        if frame is None:
            print(f"警告: 无法读取图片 '{frame_file}'，跳过")
            continue
        
        # 转为灰度图
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 使用NumPy向量化操作进行二值化
        # 亮度 >= threshold 为 '#'，否则为 ' '
        frame_chars = np.where(gray >= threshold, '#', ' ')
        
        # 将二维字符数组展平为一维字符串
        frame_text = ''.join(frame_chars.flatten())
        all_frames_text += frame_text
        
        processed_count += 1
        if processed_count % 100 == 0:
            print(f"已处理 {processed_count}/{len(frame_files)} 张图片")
    
    print(f"成功处理 {processed_count} 张图片")
    
    # 写入文件（严格按指定格式）
    print(f"正在写入文件: {output_path}")
    with open(output_path, 'w', encoding='utf-8') as f:
        # 第1行：宽度（字符数）
        f.write(f"{width}\n")
        # 第2行：高度（字符数）
        f.write(f"{height}\n")
        # 第3行：所有帧的ASCII字符（连续无换行）
        f.write(all_frames_text)
    
    # 验证文件格式
    print(f"\n输出验证:")
    print(f"- 每帧宽度: {width} 字符")
    print(f"- 每帧高度: {height} 字符")
    print(f"- 每帧字符数: {width * height}")
    print(f"- 总帧数: {processed_count}")
    print(f"- 总字符数: {len(all_frames_text)}")
    print(f"- 计算应得字符数: {width * height * processed_count}")
    print(f"- 实际字符数: {len(all_frames_text)}")
    
    if len(all_frames_text) == width * height * processed_count:
        print("✓ 格式验证通过!")
    else:
        print("✗ 格式验证失败!")
    
    return True

def verify_output(output_path, width, height, sample_frames=3):
    """
    验证输出文件格式是否正确
    """
    if not os.path.exists(output_path):
        print(f"错误: 输出文件不存在 '{output_path}'")
        return False
    
    with open(output_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    if len(lines) != 3:
        print(f"错误: 文件行数应为3，实际为{len(lines)}")
        return False
    
    # 检查前两行是否为数字
    try:
        file_width = int(lines[0].strip())
        file_height = int(lines[1].strip())
    except ValueError:
        print("错误: 前两行不是有效的数字")
        return False
    
    if file_width != width or file_height != height:
        print(f"错误: 尺寸不匹配。预期{width}x{height}，文件记录{file_width}x{file_height}")
        return False
    
    # 检查第三行字符数
    content = lines[2].strip('\n')
    chars_per_frame = width * height
    total_frames = len(content) // chars_per_frame
    
    print(f"验证结果:")
    print(f"- 文件宽度: {file_width}")
    print(f"- 文件高度: {file_height}")
    print(f"- 每帧字符数: {chars_per_frame}")
    print(f"- 推断总帧数: {total_frames}")
    print(f"- 实际总字符数: {len(content)}")
    
    # 显示前几帧的预览
    if sample_frames > 0:
        print(f"\n前{sample_frames}帧预览:")
        for i in range(min(sample_frames, total_frames)):
            start = i * chars_per_frame
            end = start + chars_per_frame
            frame_data = content[start:end]
            
            print(f"\n第{i+1}帧 (位置 {start}:{end}):")
            print("+" + "-" * width + "+")
            for row in range(0, len(frame_data), width):
                if row // width < 5 or row // width >= height - 5:
                    row_text = frame_data[row:row+width]
                    print("|" + row_text + "|")
                elif row // width == 5:
                    print("|" + "..." + " " * (width - 6) + "|")
            print("+" + "-" * width + "+")
    
    return True

def main():
    if len(sys.argv) < 2:
        print("用法: python process_frames.py <图片目录> [输出文件] [阈值]")
        print("示例: python process_frames.py frames output.txt 127")
        return
    
    frames_dir = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else "output.txt"
    threshold = int(sys.argv[3]) if len(sys.argv) > 3 else 127
    
    if not os.path.exists(frames_dir):
        print(f"错误: 目录 '{frames_dir}' 不存在")
        return
    
    # 处理图片序列
    success = images_to_ascii(frames_dir, output_path, threshold)
    
    if success:
        # 验证输出
        print("\n" + "="*50)
        print("开始验证输出文件格式...")
        
        # 读取第一张图片获取实际尺寸
        frame_files = sorted([f for f in os.listdir(frames_dir) if f.lower().endswith(('.jpg', '.jpeg', '.png'))])
        if frame_files:
            first_img = cv2.imread(os.path.join(frames_dir, frame_files[0]))
            if first_img is not None:
                height, width = first_img.shape[:2]
                verify_output(output_path, width, height, sample_frames=2)

if __name__ == "__main__":
    main()