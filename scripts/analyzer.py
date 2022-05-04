# simple script for benchmark output analysis
# outputs graphs used in thesis
import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os

# colors
COL1 = "#fbc02d"
COL2 = "#039be5"
COL3 = "#4caf50"

MAIN_COL = "#4caf50"


def GenBasic(df, name, out="graphs"):
    """Generates basic graph containg max, min and avg FPS for different scenes"""

    labels = ["Light field | Complex", "Light field | Basic",
              "Standard | Complex", "Standard | Basic", "Light field | Disco"]
    labels.reverse()
    gmin, gmax = 0, 250
    fig, ax = plt.subplots(1, figsize=(8, 5))
    r = df[df["variant"] == "basic"]
    r = r[r["particles"] == 10000]
    r = r[r["resolution"] != 1000]
    r = r[r["camera_rotation"] == True]
    a = r[["name", "FPS"]]

    print("10 000")
    print("\nMINIMUM FPS")
    minn = a.groupby(["name"]).min()
    print(minn)
    print("\nMAXIMUM FPS")
    maxx = a.groupby(["name"]).max()
    print(maxx)
    avgg = a.groupby(["name"]).mean()
    print("\nAVG FPS")
    print(avgg)
    a = pd.DataFrame()
    a["Maximum"] = maxx
    a["Průměr"] = avgg
    a["Minimum"] = minn
    a.plot(kind='barh', ax=ax,
           title=f"{name} Průměrné snímky za sekundu pro jednotlivé scény", color=[COL1, COL2, COL3])
    ax.set_xlim([gmin, gmax])
    ax.yaxis.set_ticklabels(labels)
    ax.set_ylabel("Scény")
    ax.legend(loc='center right')
    plt.savefig(f"{out}/graph{name}basic.pdf", bbox_inches="tight")


def GenParticles(df, name, out="graphs"):
    """Generates graph showing dependecy of particle count to FPS"""
    r = df[df["variant"] == "particles"]
    r = r[["particles", "FPS"]]
    r = r.groupby(["particles"]).mean()
    r.plot(color=MAIN_COL)
    plt.savefig(f"{out}/particles.png", bbox_inches="tight")


def GenResolution(df, name, out="graphs"):
    """Creates graph showing what effect has increasing VRAM on FPS """
    fig, ax = plt.subplots(figsize=(4, 4))
    r = df[df["variant"] == "resolution"]
    plt.xticks(rotation=-45)

    a = r[["resolution", "FPS"]]
    a = a.groupby(["resolution"]).mean()
    ax.set_ylim(0, 600)
    ax.plot(a, COL3)
    b = r[["resolution", "VRAM"]]
    b = b.groupby(["resolution"]).mean()
    ax.set_xlabel("Rozlíšení v pixelech")
    ax.set_ylabel("FPS")

    ax2 = ax.twinx()
    ax2.set_ylim(0, max(b["VRAM"]) + 300)
    ax2.set_ylabel("VRAM v MB")
    ax2.plot(b, COL2)
    plt.title(f"{name} | Vliv rozlišení na FPS a VRAM",  pad=20)
    plt.savefig(f"{out}/resolution.pdf", bbox_inches="tight")


if __name__ == "__main__":
    # args
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', type=str, default="result.pckl")
    parser.add_argument('--output', type=str, default="graphs")
    parser.add_argument('--name', type=str, default="")
    args = parser.parse_args()

    # read input
    if not os.path.isdir(args.output):
        os.mkdir(args.output)
    df = pd.read_pickle(args.input)

    # generate graphs
    GenBasic(df, out=args.output, name=args.name)
    GenParticles(df, out=args.output, name=args.name)
    GenResolution(df, out=args.output, name=args.name)
