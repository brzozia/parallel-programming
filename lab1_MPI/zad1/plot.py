from random import randint
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df1 = pd.read_csv('results_v03_send.txt', ";")
df2 = pd.read_csv('results_v08_v09_send.txt', ";")
df3 = pd.read_csv('results_v03_bsend.txt', ";")
df4 = pd.read_csv('results_v08_v09_bsend.txt', ";")
df1.columns = ['loops', 'msg_size', 'time', 'nan']
df2.columns = ['loops', 'msg_size', 'time', 'nan']
df3.columns = ['loops', 'msg_size', 'time', 'nan']
df4.columns = ['loops', 'msg_size', 'time', 'nan']
print(df4)
# do 3 są ok (4 jest czasem za krótkie)
# 5 dla większych wiadomości
def throughput(loops, df, fig, pl_name, gen_name):
    loops_1e4= df.loc[df['loops'] == loops]
    thr = []
    size = []
    for index, row in loops_1e4.iterrows():
        size.append(row['msg_size'])
        thr.append((2*loops*row['msg_size']*8)/row['time']/1024/1024)


    fig.add_trace(go.Scatter(
        x=size,
        y=thr,
        mode='markers',
        name=pl_name,
        marker_symbol='diamond',
        marker=dict(
            color=randint(1, 500),
            line_width=2,
            size=7,
            
        ),
    ))
    fig.update_layout(
        title=gen_name,
        xaxis_title="Rozmiar wiadomości [B]",
        yaxis_title="Przepustowość [Mbits/s]"
    )

def latency(df, msg):
    size_1 = df.loc[df['msg_size']==1]
    sum_time = size_1['time'].sum().sum()
    sum_loops = size_1['loops'].sum().sum()

    lat = sum_time/sum_loops
    print(msg+ " %.10f"%lat)



fig = go.Figure()
fig2 = go.Figure()
throughput(10000, df1, fig, "MPI_Send", "Przepustowość - vnode-03, 10000 powtórzeń")
throughput(10000, df2, fig2, "MPI_Send", "Przepustowość - vnode-08, vnode-09, 10000 powtórzeń")
throughput(10000, df3, fig, "MPI_Bsend", "Przepustowość - vnode-03, 10000 powtórzeń")
throughput(10000, df4, fig2, "MPI_Bsend", "Przepustowość - vnode-08, vnode-09, 10000 powtórzeń")
latency(df1, "Opóźnienie dla MPI_Send, vnode-03: ")
latency(df3, "Opóźnienie dla MPI_Bsend, vnode-03: ")
latency(df2, "Opóźnienie dla MPI_Send, vnode-08, vnode-09: ")
latency(df4, "Opóźnienie dla MPI_Bsend, vnode-08, vnode-09: ")
fig.show()
fig2.show()
