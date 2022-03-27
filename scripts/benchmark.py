import GPUtil
import subprocess
import psutil
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
path = "../build"
binary = "../build/Release/light_field_ps_bp.exe"

# setup process
proc = subprocess.Popen([binary, ''],
                        stdout=subprocess.PIPE, cwd=path)
proc_psutil = psutil.Process(proc.pid)


def GetPcStats():
    gpu = GPUtil.getGPUs()[0]
    res = ""
    res += "GPU load " + str(int(gpu.load*100)) + "% \t| "
    res += "VRAM usage: " + str(int(gpu.memoryUsed)) + " MB\t| "
    res += "VRAM perc: " + str(int(gpu.memoryUtil*100)) + "%\t| "
    res += "CPU usage: " + str(proc_psutil.cpu_percent(interval=None)) + "\t| "
    res += "RAM: " + \
        str(int(proc_psutil.memory_full_info().uss / 1024.0**2)) + " MB "
    return res


MAX_SAMPLES = 30
samples = 0

data = []
output = ""
while True:
    # read stdout
    line = proc.stdout.readline()
    if not line:
        break
    st = line.rstrip().decode()

    # print output
    GetPcStats()
    line = "S: " + str(int(samples / MAX_SAMPLES * 100)) + \
        "%\t | " + st + "\t | " + GetPcStats()
    print(line)
    output += line + "\n"

    # save data
    fps = int(st.split(" ")[1])
    gpu = GPUtil.getGPUs()[0]
    data.append([samples, fps, gpu.load, gpu.memoryUsed, gpu.memoryUtil,
                proc_psutil.cpu_percent(interval=None)])

    # sample counting
    samples += 1
    if samples >= MAX_SAMPLES:
        proc.terminate()
        break
df = pd.DataFrame(data=data, columns=[
                  "samples", "FPS", "GPU", "VRAM", "VRAM_UTIL", "CPU"])

file = open("result.txt", "w")
file.write(output)
file.close()

df["FPS"].plot()
print("Average values")
print(df[["FPS", "GPU", "CPU"]].mean())
plt.show()

# GPUs = GPUtil.getGPUs()
# print(GPUs[0].load)
