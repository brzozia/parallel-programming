from random import randint
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df1 = pd.read_csv('results_static.txt', ";")
df1.columns = ['threads', 'size', 'time', 'nan']
print(df1)


def throughput(df, fig, limit, gen_name):
    thr = [df.loc[df['threads'] == x] for x in range(1,5)]

    x = {}
    y = {}
    for idx, results in enumerate(thr):
        for index, row in results.iterrows():
            if idx not in x or idx not in y:
                x[idx] = []
                y[idx] = []
            if row['size'] < limit:
                x[idx].append(row['size']) 
                y[idx].append(row['time'])

    for id in range(1,5):
        fig.add_trace(go.Scatter(
            x=x[id-1][:-2],
            y=y[id-1][:-2],
            mode='markers',
            name=str(id)+" thread",
            marker_symbol='diamond',
            marker=dict(
                color=randint(1, 500),
                line_width=2,
                size=7,
            ),
        ))
    fig.update_layout(
        title=gen_name,
        xaxis_title="Ilość elementów w tablicy tablicy",
        yaxis_title="Czas trwania części równoległej [s]"
    )


fig = go.Figure()
throughput(df1, fig, 300000000.0, "Czas trwania części równoległej programu z zależności od rozmiaru problemu")
fig.show()

fig2 = go.Figure()
throughput(df1, fig2, 15000000, "Czas trwania części równoległej programu z zależności od rozmiaru problemu")
fig2.show()
