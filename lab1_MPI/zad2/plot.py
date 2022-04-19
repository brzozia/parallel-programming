from random import randint
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df1 = pd.read_csv('results_promet_v19.txt', ";")
df2 = pd.read_csv('results_promet_v20.txt', ";")

df3 = pd.read_csv('results_promet_v14.txt', ";")
df4 = pd.read_csv('results_promet_v15.txt', ";")
df5 = pd.read_csv('results_promet_v16.txt', ";")
df6 = pd.read_csv('results_promet_v17.txt', ";")
df7 = pd.read_csv('results_promet_v18.txt', ";")

df8 = pd.read_csv('results_promet_v9.txt', ";")
df9 = pd.read_csv('results_promet_v10.txt', ";")
df10 = pd.read_csv('results_promet_v11.txt', ";")
df11 = pd.read_csv('results_promet_v12.txt', ";")
df12 = pd.read_csv('results_promet_v13.txt', ";")
df13 = pd.read_csv('results_promet_v21.txt', ";")
df14 = pd.read_csv('results_promet_v22.txt', ";")
df15 = pd.read_csv('results_promet_v23.txt', ";")
df16 = pd.read_csv('results_promet_v24.txt', ";")
df17 = pd.read_csv('results_promet_v25.txt', ";")
df18 = pd.read_csv('results_promet_v26.txt', ";")

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
df13.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df14.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df15.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df16.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df17.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]
df18.columns = ['proc', 'all_points', 'my_points', 'inside_points', 'start', 'parall', 'end', 'pi', "none"]

df = [df1, df2, df3, df4, df5, df6,df7, df8, df9, df10, df11, df12, df13, df14, df15, df16, df17, df18]

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
    y1std = []
    y2std = []
    y3std = []
    for key,d in all_df.items():
        x1.append(key)

        t1 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==4000000]
        t2 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==300000000]
        t3 = [x['end'] - x['start'] for i, x in d.iterrows() if x['all_points']==5000000000]

        y1.append(sum(t1)/len(t1))
        y1std.append((sum([((x - sum(t1)/len(t1)) ** 2) for x in t1]) / len(t1)) ** 0.5)
        y2.append(sum(t2)/len(t2))
        y2std.append((sum([((x - sum(t2)/len(t2)) ** 2) for x in t2]) / len(t2)) ** 0.5)
        y3.append(sum(t3)/len(t3))   #albo inaczej + odchylenie
        y3std.append((sum([((x - sum(t3)/len(t3)) ** 2) for x in t3]) / len(t3)) ** 0.5)

    
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
        name="5*10^9",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))

    fig.add_trace(go.Scatter(
        x=x1,
        y=y1std,
        mode='markers',
        name="odchylenie standardowe 4*10^6",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y2std,
        mode='markers',
        name="odchylenie standardowe 3*10^8",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y3std,
        mode='markers',
        name="odchylenie standardowe 5*10^9",
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
        name="5*10^9",
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



def sekw(fig, x1, yz1, yz2, yz3):
    x1 = x1[1:]
    y1 = []
    y2 = []
    y3 = []
    for i in range(1, 12,1):
        y1.append(((1/yz1[i]) - (1/(i+1)) ) / (1 - (1/(i+1))))
        y2.append(((1/yz2[i]) - (1/(i+1)) ) / (1 - (1/(i+1))))
        y3.append(((1/yz3[i]) - (1/(i+1)) ) / (1 - (1/(i+1))))
    
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
        y=[0 for x in x1],
        mode='lines',
        name="idealna część sekwencyjna",
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
        name="5*10^9",
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
        ),
    ))
    fig.update_layout(
        title="Część sekwencyjna programu",
        xaxis_title="Ilość procesorów",
        yaxis_title="Część sekwencyjna"
    )

    return x1, y1, y2, y3


def efectivity(fig, x1, y1, y2, y3):
    y1 = [y1 / x1 for y1,x1 in zip(y1, x1)]
    y2 = [y2 / x1 for y2,x1 in zip(y2, x1)]
    y3 = [y3 / x1 for y3,x1 in zip(y3, x1)]

    fig.add_trace(go.Scatter(
        x=x1,
        y=y1,
        mode='markers',
        name="4*10^6",
        marker_symbol='diamond',
        marker=dict(
            color='orange',
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
        y=[1 for x in x1],
        mode='lines',
        name="idealna efektywność",
        marker_symbol='diamond',
        marker=dict(
            color='orange',
            line_width=2,
            size=7,
        ),
    ))
    fig.add_trace(go.Scatter(
        x=x1,
        y=y3,
        mode='markers',
        name="5*10^9",
        marker_symbol='diamond',
        marker=dict(
            color='green',
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
x11, y11, y21, y31 = speedup(fig2, x1, y1, y2, y3)
sekw(fig4, x11, y11, y21, y31)
efectivity(fig3, x11, y11, y21, y31)


fig.show()
fig2.show()
fig3.show()
fig4.show()