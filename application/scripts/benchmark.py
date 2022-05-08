# script for benchamrking main application
# for help run:
# python benchmark.py -h
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


def GetPcStats(proc) -> str:
    """Get current PC statistics"""
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
    """
    Runs single benchmark
    * particles - amount of rendered particles 
    * resolution - lf texture resolution
    * variant - test batch name, e.g "basic" or "particles"
    * scene - name of the scene
    * rotate_camera - if true, camera will rotate around the scene

    Returns pandas dataframe containging messurements
    """

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


if __name__ == "__main__":
    # ==== arguments ====
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--iterations', type=int)
    parser.add_argument('--cwd', type=str, default=path)
    parser.add_argument('--bin', type=str, default=binary)
    args = parser.parse_args()
    path = args.cwd
    if path == ".":
        path = None
    binary = args.bin
    # args - iterations
    iterations = 10
    if (args.iterations != None):
        iterations = args.iterations
    MAX_SAMPLES = iterations
    print(f"Starting with {MAX_SAMPLES} samples per scene...")

    # == basic run ==
    scenes = ["s3d", "sbench", "sbenchc", "s3dc", "disco"]
    particles = [1000, 4000, 7000, 10000]
    resolutions = [1000, 5000]
    main_df = pd.DataFrame()  # dataframe containg all results
    # run tests for both rotating and not rotating camera
    for rotating_camera in [True, False]:
        for scene in scenes:
            for particle_amount in particles:

                if scene == "s3d" or scene == "s3dc":
                    df = RunBenchmark(particle_amount, 0, "basic",
                                      scene=scene, rotate_camera=rotating_camera)
                    main_df = pd.concat([main_df, df])
                else:  # standard 3d scenes are without resolution
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
