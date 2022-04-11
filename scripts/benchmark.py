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


MAX_SAMPLES = 10

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
        name = f"{scene}_p{particles}_r{resolution}"
        data.append([name, scene, particles, resolution, samples, fps, gpu.load, gpu.memoryUsed, gpu.memoryUtil,
                    proc_psutil.cpu_percent(interval=None)])

        # sample counting
        samples += 1
        if samples >= MAX_SAMPLES:
            proc.terminate()
            break
    df = pd.DataFrame(data=data, columns=[
        "name", "scene", "particles", "resolution", "samples", "FPS", "GPU", "VRAM", "VRAM_UTIL", "CPU"])
    return df


main_df = pd.DataFrame()

df = RunBenchmark(1000, 5000, "s3d")
main_df = pd.concat([main_df, df])
df = RunBenchmark(4000, 5000, "s3d")
main_df = pd.concat([main_df, df])
df = RunBenchmark(7000, 5000, "s3d")
main_df = pd.concat([main_df, df])
df = RunBenchmark(10000, 5000, "s3d")
main_df = pd.concat([main_df, df])

df = RunBenchmark(1000, 5000)
main_df = pd.concat([main_df, df])
df = RunBenchmark(4000, 5000)
main_df = pd.concat([main_df, df])
df = RunBenchmark(7000, 5000)
main_df = pd.concat([main_df, df])
df = RunBenchmark(10000, 5000)
main_df = pd.concat([main_df, df])

df = RunBenchmark(1000, 1000)
main_df = pd.concat([main_df, df])
df = RunBenchmark(4000, 1000)
main_df = pd.concat([main_df, df])
df = RunBenchmark(7000, 1000)
main_df = pd.concat([main_df, df])
df = RunBenchmark(10000, 1000)
main_df = pd.concat([main_df, df])

# complex
df = RunBenchmark(1000, 1000, "sbenchc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(4000, 1000, "sbenchc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(7000, 1000, "sbenchc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(10000, 1000, "sbenchc")
main_df = pd.concat([main_df, df])

df = RunBenchmark(1000, 5000, "sbenchc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(4000, 5000, "sbenchc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(7000, 5000, "sbenchc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(10000, 5000, "sbenchc")
main_df = pd.concat([main_df, df])

df = RunBenchmark(1000, 5000, "s3dc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(4000, 5000, "s3dc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(7000, 5000, "s3dc")
main_df = pd.concat([main_df, df])
df = RunBenchmark(10000, 5000, "s3dc")
main_df = pd.concat([main_df, df])


print("Done")
#print("Average values")
#print(df[["FPS", "GPU", "CPU"]].mean())
#ax = plt.subplot()
main_df.to_pickle("result.pckl")
#main_df = main_df.mean()
print(main_df)
r = main_df[["name", "FPS"]]
r = r.groupby(["name"]).mean()
print(r)
r = r.sort_values("FPS")
r.plot(kind='barh')
#ax.set_ylim([0, None])
plt.show()

# GPUs = GPUtil.getGPUs()
# print(GPUs[0].load)
