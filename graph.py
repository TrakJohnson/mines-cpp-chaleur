import matplotlib.pyplot as plt
import pandas as pd

m = pd.read_csv("build/test.txt", sep=' ').to_numpy()
for row in m:
    print(" ".join(map(str, row)))
    plt.plot(range(len(row)), row)
plt.show()
