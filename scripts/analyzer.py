import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_pickle("result.pckl")
#main_df = main_df.mean()
print(df)
r = df[["name", "FPS"]]
r = r.groupby(["name"]).mean()
print(r)
r = r.sort_values("FPS")
r.plot(kind='barh')
#ax.set_ylim([0, None])
plt.show()
