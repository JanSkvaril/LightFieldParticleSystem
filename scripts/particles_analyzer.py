import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os

pallete = ["#fd7f6f", "#7eb0d5", "#b2e061", "#bd7ebe",
           "#ffb55a", "#ffee65", "#beb9db", "#fdcce5", "#8bd3c7"]


def GetParticles(df):
    r = df[df["variant"] == "particles"]
    r = r[["particles", "FPS"]]
    r = r[r["particles"] < 10000]
    r = r.groupby(["particles"]).mean()
    return r


df1 = pd.read_pickle("result.pckl")
df2 = pd.read_pickle("result2.pckl")
df3 = pd.read_pickle("result3.pckl")
df4 = pd.read_pickle("result4.pckl")

fig, ax = plt.subplots(1)
df = pd.DataFrame()
df["Sestava S1"] = GetParticles(df1)
df["Sestava S2"] = GetParticles(df2)
df["Sestava S3"] = GetParticles(df3)
df["Sestava S4"] = GetParticles(df4)
df.plot(ax=ax, color=pallete, title="Vliv počtu částic na FPS jednotlivých sestav")
ax.set_xlabel("Počet částic")
ax.set_ylabel("Počet snímků za sekundu (FPS)")
plt.savefig("particle_count.pdf")
