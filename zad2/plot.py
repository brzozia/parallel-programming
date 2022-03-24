from random import randint
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df1 = pd.read_csv('results_promet_v2.txt', ";")
df2 = pd.read_csv('results_promet_v3.txt', ";")
df3 = pd.read_csv('results_promet_v4.txt', ";")
df4 = pd.read_csv('results_promet_v5.txt', ";")
df5 = pd.read_csv('results_promet_v6.txt', ";")
df6 = pd.read_csv('results_promet_v7.txt', ";")
df7 = pd.read_csv('results_promet_v8.txt', ";")
df8 = pd.read_csv('results_promet_v9.txt', ";")
df9 = pd.read_csv('results_promet_v10.txt', ";")
df10 = pd.read_csv('results_promet_v11.txt', ";")
df11 = pd.read_csv('results_promet_v12.txt', ";")
df12 = pd.read_csv('results_promet_v13.txt', ";")

df1.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df2.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df3.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df4.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df5.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df6.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df7.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df8.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df9.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df10.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df11.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df12.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]

df = [df1, df2, df3, df4, df5, df6,df7, df8, df9, df10, df11, df12]

all_df = {}

for i in range(1,13,1):
    arr = pd.DataFrame(columns=['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"])
    for d in df:
        arr = pd.concat([arr, d.loc[d['proc'] == i]], axis=0,sort=False)

    all_df[i] = arr
    

def time(fig):
    x1 = []
    y1 = []
    y2 = []
    y3 = []
    for key,d in all_df.items():
        x1.append(key)

        t1 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==4000000]
        t2 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==300000000]
        t3 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==50000000000]

        y1.append(sum(t1)/len(t1))
        y2.append(sum(t2)/len(t2))
        y3.append(sum(t3)/len(t3))   #albo inaczej + odchylenie

    
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
        name="5*10^10",
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
    y2 = [yz2[0]/x for x in yz2]
    y3 = [yz3[0]/x for x in yz3]

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
        name="5*10^10",
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



def sekw(fig):
    x1 = []
    y1 = []
    y2 = []
    y3 = []

    tp1 = -1
    tp2 = -1
    tp3 = -1
    for key,d in all_df.items():
        x1.append(key)

        t1 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==4000000]
        t2 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==300000000]
        t3 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==50000000000]

        

        if(tp1==-1):
            tp1 =sum(t1)/len(t1)
            tp2 = sum(t2)/len(t2)
            tp3 = sum(t3)/len(t3)
    
        y1.append(tp1/(sum(t1)/len(t1)))
        y2.append(tp2/(sum(t2)/len(t2)))
        y3.append(tp3/(sum(t3)/len(t3))) #albo inaczej + odchylenie

    
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
efectivity(fig3, x1, y1, y2, y3)
sekw(fig4)

fig.show()
fig2.show()
fig3.show()
fig4.show()