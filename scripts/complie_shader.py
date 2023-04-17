import os
import sys
import subprocess


cur_path = os.path.split(os.path.realpath(__file__))[0]
shader_types = ["frag","vert"]
shader_dir = os.path.normpath(cur_path + "/../engine/shader/glsl")
output_dir = os.path.join(shader_dir,"compile")
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

if sys.platform == "win32":
    compiler_path = os.path.normpath(cur_path + "/../engine/3rdparty/VulkanSDK/bin/Win32/glslangValidator.exe")
    for file in os.listdir(shader_dir):
        if file.split('.')[-1] in shader_types:
            shader_path = os.path.join(shader_dir,file)
            output_path = os.path.join(output_dir,file + ".spv")
            cmd = compiler_path + " -V -o " + output_path + " " + shader_path
            output = subprocess.check_output(cmd, shell=True)
else:
    raise OSError

