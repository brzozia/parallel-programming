from random import randint
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df1 = pd.read_csv('results_2_vcl_v7.txt', ";")


df1.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]


df = [df1]

all_df = {}

for i in range(1,13,1):
    arr = pd.DataFrame(columns=['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"])
    for d in df:
        #  for i, x in d.iterrows:
        arr = pd.concat([arr, d.loc[d['proc'] == i]], axis=0,sort=False)

    all_df[i] = arr

size = 800000000

def time(fig):
    x1 = []
    y1 = []
    y2 = []
    y3 = []

    for key,d in all_df.items():
        print(d)
        x1.append(key)

        t1 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==size]


        print(t1)
        y1.append(sum(t1)/len(t1))

    
    fig.add_trace(go.Scatter(
        x=x1,
        y=y1,
        mode='markers',
        name="zmierzony czas (8*10^8 points)",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    
    fig.update_layout(
        title="Czas trwania programu",
        xaxis_title="Ilość procesorów",
        yaxis_title="Czas [s]"
    )

    return x1, y1, y2, y3

def speedup(fig, x1, yz1, yz2, yz3):
    y1 = [yz1[0]/x for x in yz1]
    y2 = []
    y3 = []

    fig.add_trace(go.Scatter(
        x=x1,
        y=y1,
        mode='markers',
        name="zmierzone przyspieszenie (8*10^8 punktów)",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=x1,
        mode='lines',
        name="idealne przyspieszenie",
        # marker_symbol='diamond',
        fillcolor='yellow',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    
    fig.update_layout(
        title="Przyspieszenie programu",
        xaxis_title="Ilość procesorów",
        yaxis_title="Przyspieszenie"
    )

    return x1, y1, y2, y3



def sekw(fig):
    x1 = []
    y1 = []
    y2 = []
    y3 = []

    tp1 = -1
    tp2 = -1
    tp3 = -1
    for d in all_df:
        x1.append(d['proc'][1])

        t1 = [x['end'] - x['parall'] for i, x in d.iterrows if x['all_points']==4000000]
        t2 = [x['end'] - x['parall'] for i, x in d.iterrows if x['all_points']==300000000]
        t3 = [x['end'] - x['parall'] for i, x in d.iterrows if x['all_points']==50000000000]

        if(tp1==-1):
            tp1 = t1.sum()/t1.size
            tp2 = t2.sum()/t2.size
            tp3 = t3.sum()/t3.size
    
        y1.append(tp1/(t1.sum()/t1.size))
        y2.append(tp2/(t2.sum()/t2.size))
        y3.append(tp3/(t3.sum()/t3.size))  #albo inaczej + odchylenie

    
    fig.add_trace(go.Scatter(
        x=x1,
        y=y1,
        mode='markers',
        name="4*10^6",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y2,
        mode='markers',
        name="3*10^8",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y3,
        mode='markers',
        name=5*10^10,
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.update_layout(
        title="Przyspieszenie programu",
        xaxis_title="Ilość procesorów",
        yaxis_title="Przyspieszenie"
    )

    return x1, y1, y2, y3


def efectivity(fig, x1, y1, y2, y3):
    y1 = y1/x1
    y2 = y2/x1
    y3 = y3/x1

    fig.add_trace(go.Scatter(
        x=x1,
        y=y1,
        mode='markers',
        name="4*10^6",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y2,
        mode='markers',
        name="3*10^8",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y3,
        mode='markers',
        name=5*10^10,
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.update_layout(
        title="Efektywność programu",
        xaxis_title="Ilość procesorów",
        yaxis_title="Efektywność"
    )





fig = go.Figure()
fig2 = go.Figure()
fig3 = go.Figure()
fig4 = go.Figure()

x1, y1, y2, y3 = time(fig)
x1, y1, y2, y3 = speedup(fig2, x1, y1, y2, y3)
# efectivity(fig3, x1, y1, y2, y3)
# sekw(fig4)

fig.show()
fig2.show()
# fig3.show()
# fig4.show()