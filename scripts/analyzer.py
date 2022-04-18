import pandas as pd
import matplotlib.pyplot as plt


COL1 = "#22a200"
COL2 = "#008ca2"


def GenBasic(df):
    fig, ax = plt.subplots()
    r = df[df["variant"] == "basic"]
    r = r[r["particles"] == 10000]
    r = r[r["resolution"] != 1000]

    a = r[r["camera_rotation"] == True]
    a = a[["name", "FPS"]]
    a = a.groupby(["name"]).mean()
    a = a.sort_values("FPS")
    a.plot(kind='barh', ax=ax, color=COL1)
    plt.savefig("graphs/basic.png", bbox_inches="tight")


def GenParticles(df):
    r = df[df["variant"] == "particles"]
    r = r[["particles", "FPS"]]
    r = r.groupby(["particles"]).mean()
    r.plot()
    plt.savefig("graphs/particles.png", bbox_inches="tight")


def GenResolution(df):
    fig, ax = plt.subplots()
    r = df[df["variant"] == "resolution"]
    a = r[["resolution", "FPS"]]
    a = a.groupby(["resolution"]).mean()
    ax.plot(a, COL1)
    b = r[["resolution", "VRAM"]]
    b = b.groupby(["resolution"]).mean()
    ax2 = ax.twinx()
    ax2.plot(b, COL2)
    plt.savefig("graphs/resolution.png", bbox_inches="tight")


df = pd.read_pickle("result.pckl")

print(df)
GenBasic(df)
GenParticles(df)
GenResolution(df)
