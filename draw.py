import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# Before clustering
df = pd.read_csv("data.csv", header=None)
df.columns = ["x", "y"]
sns.scatterplot(x=df["x"], 
                y=df["y"])
plt.title("Raw Data")

# After clustering
plt.figure()
df = pd.read_csv("output.csv")
sns.scatterplot(x=df.x, y=df.y, 
                hue=df.c,
                palette=sns.color_palette("hls", n_colors=5))
plt.xlabel("x")
plt.ylabel("y")
plt.title("Clustered")

plt.show()
