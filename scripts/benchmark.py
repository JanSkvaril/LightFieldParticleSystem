from pickle import FALSE
import GPUtil
import subprocess
import psutil
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import argparse
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


def RunBenchmark(particles, resolution, variant, scene="sbench", rotate_camera=False):
    print(
        f" ==== Running test with [{particles}] particles, resolution [{resolution}] and [{scene}] scene. Camera rotation: {rotate_camera} ====")

    flags = [binary, '-f', "-n",
             f"-p {particles}", f"-r {resolution}", f"--{scene}"]
    if rotate_camera:
        flags.append("-c")
    # setup process
    proc = subprocess.Popen(flags,
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

        # save data
        fps = int(st.split(" ")[1])
        gpu = GPUtil.getGPUs()[0]
        name = f"{scene}_p{particles}_r{resolution}"
        data.append([name, variant, scene, particles, resolution, samples, rotate_camera, fps, gpu.load, gpu.memoryUsed, gpu.memoryUtil,
                    proc_psutil.cpu_percent(interval=None)])

        # sample counting
        samples += 1
        if samples >= MAX_SAMPLES:
            proc.terminate()
            break
    df = pd.DataFrame(data=data, columns=[
        "name", "variant", "scene", "particles", "resolution", "samples", "camera_rotation", "FPS", "GPU", "VRAM", "VRAM_UTIL", "CPU"])
    return df


# args
parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--iterations', type=int)
args = parser.parse_args()
# args - iterations
iterations = 10
if (args.iterations != None):
    iterations = args.iterations
MAX_SAMPLES = iterations
print(f"Starting with {MAX_SAMPLES} samples per scene...")
# == basic run ==
scenes = ["disco", "s3d", "sbench", "sbenchc", "s3dc"]
particles = [1000, 4000, 7000, 10000]
resolutions = [1000, 5000]
main_df = pd.DataFrame()
for rotating_camera in [True, False]:
    for scene in scenes:
        for particle_amount in particles:
            # standard 3d scenes are without resolution
            if scene == "s3d" or scene == "s3dc":
                df = RunBenchmark(particle_amount, 0, "basic",
                                  scene=scene, rotate_camera=rotating_camera)
                main_df = pd.concat([main_df, df])
            else:
                for resolution in resolutions:
                    df = RunBenchmark(particle_amount, resolution, "basic",
                                      scene=scene, rotate_camera=rotating_camera)
                    main_df = pd.concat([main_df, df])

# == resolution ==
for resolution in range(1000, 30000, 1000):
    PARTICLE_AMOUNT = 3000
    df = RunBenchmark(PARTICLE_AMOUNT,
                      resolution, "resolution")
    main_df = pd.concat([main_df, df])

# == particles ==
for particles in range(1000, 50000, 1000):
    PARTICLE_RESOLUTION = 1000
    df = RunBenchmark(particles, PARTICLE_RESOLUTION,
                      "particles", rotate_camera=True)
    main_df = pd.concat([main_df, df])

print("Done")
main_df.to_pickle("result.pckl")
print("Saved to [result.pckl]")
