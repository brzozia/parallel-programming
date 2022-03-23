import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df = pd.read_csv('results_1.txt', ";")
df.columns = ['loops', 'msg_size', 'time', 'nan']

# powt_10000 = [x for x in df if df['loops']==10000]
def throughput(loops):
    loops_1e4= df.loc[df['loops'] == loops]
    thr = []
    size = []
    for index, row in loops_1e4.iterrows():
        size.append(row['msg_size'])
        print(2*loops*row['msg_size']/(row['time']))
        thr.append(2*loops*row['msg_size']/(row['time']*1024*1024))
    print(thr)

    fig = go.Figure()
    fig.add_trace(go.Scatter(
        x=size,
        y=thr,
        mode='markers',
        name='Przepustowość',
        marker_symbol='diamond',
        marker=dict(
            color='blue',
            line_width=2,
            size=7,
        )
    ))
    fig.update_layout(
        title="Przepustowość (dla "+str(loops) +" powtórzeń komunikacji",
        xaxis_title="rozmiar wiadomości [B]",
        yaxis_title="przepustowość"
    )

    fig.show()


throughput(100000)
# fig = go.Figure()
# fig.add_trace(go.Scatter(
#     x=buy_trades_time,
#     y=buy_trades_price,
#     mode='markers',
#     name='buy trades',
#     marker_symbol='diamond',
#     marker=dict(
#         color='blue',
#         line_width=2,
#         size=7,

#     )
# ))

# fig.add_trace(go.Scatter(
#     x=sell_trades_time,
#     y=sell_trades_price,
#     mode='markers',
#     name='sell trades',
#     marker_symbol='square',
#     marker=dict(
#         color='yellow',
#         line_width=2,
#         size=7
#     )
# ))

# fig.update_layout(
#     title="Candles",
#     xaxis_title="time",
#     yaxis_title="price",
#     width=width,
#     height=height
# )

# fig.show()