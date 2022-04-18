import pandas as pd
import matplotlib.pyplot as plt


COL1 = "#22a200"
COL2 = "#008ca2"


def GenBasic(df):
    labels = ["Light field | Complex", "Light field | Basic",
              "Standard | Complex", "Standard | Basic", "Light field | Disco"]
    labels.reverse()

    fig, ax = plt.subplots(2)
    r = df[df["variant"] == "basic"]
    r = r[r["particles"] == 10000]
    r = r[r["resolution"] != 1000]

    r = r[r["camera_rotation"] == True]
    a = r[["name", "FPS"]]
    a = a.groupby(["name"]).mean()
    a.plot(kind='barh', ax=ax[0], color=COL1)
    ax[0].yaxis.set_ticklabels(labels)

    b = r[["name", "VRAM"]]
    b = b.groupby(["name"]).mean()
    b.plot(kind='barh', ax=ax[1], color=COL2)
    ax[1].yaxis.set_ticklabels(labels)

    plt.savefig("graphs/basic.png", bbox_inches="tight")


def GenParticles(df):
    r = df[df["variant"] == "particles"]
    r = r[["particles", "FPS"]]
    r = r.groupby(["particles"]).mean()
    r.plot(color=COL1)
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
