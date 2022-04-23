import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
COL1 = "#fbc02d"
COL2 = "#039be5"
COL3 = "#4caf50"

MAIN_COL = "#4caf50"


def GenBasic(df, name, out="graphs"):
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
    ax.set_ylabel("10 000 částic")
    ax.legend(loc='center left', bbox_to_anchor=(1.0, 0.5))

    # r = df[df["variant"] == "basic"]
    # r = r[r["particles"] == 4000]
    # r = r[r["resolution"] != 1000]
    # r = r[r["camera_rotation"] == True]
    # a = r[["name", "FPS"]]

    # print("4 000")
    # print("\nMINIMUM FPS")
    # minn = a.groupby(["name"]).min()
    # print(minn)
    # print("\nMAXIMUM FPS")
    # maxx = a.groupby(["name"]).max()
    # print(maxx)
    # avgg = a.groupby(["name"]).mean()
    # print("\nAVG FPS")
    # print(avgg)
    # a = pd.DataFrame()
    # a["Maximum"] = maxx
    # a["Průměr"] = avgg
    # a["Minimum"] = minn

    # a.plot(kind='barh', ax=ax[1], color=[COL1, COL2, COL3])
    # ax[1].yaxis.set_ticklabels(labels)
    # ax[1].set_xlim([gmin, gmax])
    # ax[1].set_xlabel("FPS - Počet snímků za sekundu")
    # ax[1].set_ylabel("4 000 částic")
    # ax[1].legend(loc='center left', bbox_to_anchor=(1.0, 0.5))

    # # b = r[["name", "VRAM_UTIL"]]
    # # b = b.groupby(["name"]).mean()
    # # b.plot(kind='barh', ax=ax[1], color=COL2)
    # # ax[1].yaxis.set_ticklabels(labels)
    plt.savefig(f"{out}/graph{name}basic.pdf", bbox_inches="tight")


def GenParticles(df, name, out="graphs"):
    r = df[df["variant"] == "particles"]
    r = r[["particles", "FPS"]]
    r = r.groupby(["particles"]).mean()
    r.plot(color=MAIN_COL)
    plt.savefig(f"{out}/particles.png", bbox_inches="tight")


def GenResolution(df, name, out="graphs"):
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


parser = argparse.ArgumentParser()
parser.add_argument('--input', type=str, default="result.pckl")
parser.add_argument('--output', type=str, default="graphs")
parser.add_argument('--name', type=str, default="")
args = parser.parse_args()
if not os.path.isdir(args.output):
    os.mkdir(args.output)
df = pd.read_pickle(args.input)

# print(df)
GenBasic(df, out=args.output, name=args.name)
GenParticles(df, out=args.output, name=args.name)
GenResolution(df, out=args.output, name=args.name)
