import sys
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd


### Données exportées par le C++
FILENAME = sys.argv[1]
NUMBER_LINES = 100
REVERSE = True  # permet d'avoir les premières courbes au-dessus des dernières
                # ce qui rend le tout plus lisible d'un pdv temporel

# metadata
with open(FILENAME) as f:
    (t0, tN, deltaTime), (x0, xN, deltaX), *_ = [
        list(map(float, i.split(" "))) for i in f.readlines()
    ]

# data
df = pd.read_csv(FILENAME, skiprows=[0,1], sep=" ")
# on ne plot pas toutes les lignes, on limite au maximum à NUMBER_LINES
df = df.iloc[::max(int(len(df)/NUMBER_LINES), 1)]
m = df.to_numpy()


### Plot des résultats
fig, ax = plt.subplots(1, 2, figsize=(6, 1),
                       gridspec_kw={'width_ratios': [15, 1]})
n = len(m)
cmap = plt.cm.turbo
x_colors = np.linspace(t0, tN+1, n)
colors = cmap(x_colors)

# Rampe de couleurs
norm = mpl.colors.Normalize(vmin=t0, vmax=tN)
cb1 = mpl.colorbar.ColorbarBase(ax[1], cmap=cmap,
                                norm=norm,
                                orientation='vertical')
cb1.set_label('temps (s)')


# plot
if REVERSE:
    m = m[::-1]
    colors = colors[::-1]
for row, c in zip(m, colors):
    ax[0].plot(np.linspace(x0, xN, len(row)), row, c=c)

ax[0].set_xlabel('x')
ax[0].set_xlim([x0, xN])
ax[0].set_ylabel('Température (K)')
# TODO: ne marche que sur linux :P
ax[0].set_title(
    f"{FILENAME.split('/')[-1].split('.')[0].replace('_', ' ')} dt={deltaTime} dx={deltaX}"
)
plt.show()
