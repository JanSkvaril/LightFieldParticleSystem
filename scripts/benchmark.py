import GPUtil
import subprocess
import psutil
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
path = "../build"
binary = "../build/Release/light_field_ps_bp.exe"


def GetPcStats(proc):
    gpu = GPUtil.getGPUs()[0]
    res = ""
    res += "GPU load " + str(int(gpu.load*100)) + " %  \t| "
    res += "VRAM usage: " + str(int(gpu.memoryUsed)) + " MB\t| "
    res += "VRAM perc: " + str(int(gpu.memoryUtil*100)) + "%\t| "
    res += "CPU usage: " + str(proc.cpu_percent(interval=None)) + "\t| "
    res += "RAM: " + \
        str(int(proc.memory_full_info().uss / 1024.0**2)) + " MB "
    return res


MAX_SAMPLES = 15

output = ""


def RunBenchmark(particles, resolution, scene="sbench"):
    print(
        f" ======= Running test with [{particles}] particles, resolution [{resolution}] and [{scene}] scene =======")
    # setup process
    proc = subprocess.Popen([binary, '-f', "-n", f"-p {particles}", f"-r {resolution}", f"--{scene}"],
                            stdout=subprocess.PIPE, cwd=path)
    proc_psutil = psutil.Process(proc.pid)
    samples = 0
    data = []
    while True:
        # read stdout
        line = proc.stdout.readline()
        if not line:
            break
        st = line.rstrip().decode()

        # print output
        line = "S: " + str(int(samples / MAX_SAMPLES * 100)) + \
            "%\t | " + st + "\t | " + GetPcStats(proc_psutil)
        print(line)
        #output += line + "\n"

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
    return df


main_df = pd.DataFrame()


#df = RunBenchmark(1000, 5000, "s3d")
#main_df["s_1000"] = df["FPS"]
df = RunBenchmark(4000, 5000, "s3d")
main_df["s_4000"] = df["FPS"]
df = RunBenchmark(7000, 5000, "s3d")
main_df["s_7000"] = df["FPS"]
df = RunBenchmark(10000, 5000, "s3d")
main_df["s_10000"] = df["FPS"]

#df = RunBenchmark(1000, 5000)
#main_df["1000"] = df["FPS"]
df = RunBenchmark(4000, 5000)
main_df["4000"] = df["FPS"]
df = RunBenchmark(7000, 5000)
main_df["7000"] = df["FPS"]
df = RunBenchmark(10000, 5000)
main_df["10000"] = df["FPS"]

#df = RunBenchmark(1000, 1000)
#main_df["m_1000"] = df["FPS"]
df = RunBenchmark(4000, 1000)
main_df["m_4000"] = df["FPS"]
df = RunBenchmark(7000, 1000)
main_df["m_7000"] = df["FPS"]
df = RunBenchmark(10000, 1000)
main_df["m_10000"] = df["FPS"]

print("Done")
# print("Average values")
# print(df[["FPS", "GPU", "CPU"]].mean())
ax = plt.subplot()
df.to_pickle("result.pckl")
main_df.plot(ax=ax)
ax.set_ylim([0, None])
plt.show()

# GPUs = GPUtil.getGPUs()
# print(GPUs[0].load)
