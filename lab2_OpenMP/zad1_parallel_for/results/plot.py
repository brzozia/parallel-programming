from random import randint
import sched
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

def time_size(df, fig, limit, gen_name):
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
            mode='markers+lines',
            name=str(id)+" thread",
            marker_symbol='circle',
            marker=dict(
                # color=randint(1, 500),
                line_width=1,
                size=8,
            ),
            
        ))
    fig.update_xaxes(type="log")
    fig.update_yaxes(type="log")
    fig.update_layout(
        title=gen_name,
        xaxis_title="Ilość elementów w tablicy",
        yaxis_title="Czas trwania części równoległej [s]"
    )

def time_schedule(sdict, schedules, gen_name):
    
    for thr in range(0,4):
        fig = go.Figure()
        xs = {}
        ys = {}
        for name in schedules:
            res_list=sdict[name]
            df1 = res_list[thr]
            for idx, row in df1.iterrows():
                if name not in xs or name not in ys:
                    xs[name] = []
                    ys[name] = []
                xs[name].append(row['size']) 
                ys[name].append(row['time'])

        
        for name in schedules:
            fig.add_trace(go.Scatter(
                x=xs[name],
                y=ys[name],
                mode='markers+lines',
                name=name,
                marker_symbol='circle',
                marker=dict(
                    line_width=1,
                    size=8,
                ),
                
            ))
        # fig.update_xaxes(type="log")
        # fig.update_yaxes(type="log")
        fig.update_layout(
            title=gen_name+str(thr+1),
            xaxis_title="Ilość elementów w tablicy",
            yaxis_title="Czas trwania części równoległej [s]"
        )
        fig.show()
        print("one")

# def speedup(df, fig, limit, gen_name, sched_name):
#     thr = [df.loc[df['size'] == x] for x in range(1,5)]

#     x = {}
#     y = {}
#     for idx, results in enumerate(thr):
#         for index, row in results.iterrows():
#             if idx not in x or idx not in y:
#                 x[idx] = []
#                 y[idx] = []
#             if row['size'] < limit:
#                 x[idx].append(row['size']) 
#                 y[idx].append(row['time']/results.iloc[idx]['time'])

#     for id in range(1,5):
#         fig.add_trace(go.Scatter(
#             x=x[id-1][:-2],
#             y=y[id-1][:-2],
#             mode='markers+lines',
#             name=str(id)+" thread",
#             marker_symbol='circle',
#             marker=dict(
#                 # color=randint(1, 500),
#                 line_width=1,
#                 size=8,
#             ),
            
#         ))
#     fig.update_xaxes(type="log")
#     fig.update_yaxes(type="log")
#     fig.update_layout(
#         title=gen_name,
#         xaxis_title="Ilość elementów w tablicy tablicy",
#         yaxis_title="Przyspieszenie"
#     )
def speedup(df, fig, limit, gen_name):
    thr = [df.loc[df['threads'] == x] for x in range(1,5)]

    x = {}
    y = {}
    
    for idx, results in enumerate(thr):
        i=0
        for index, row in results.iterrows():
            if idx not in x or idx not in y:
                x[idx] = []
                y[idx] = []
            if row['size'] < limit:
                x[idx].append(row['size']) 
                print(thr[0].iloc[i]['time'])
                print(row['time'])
                y[idx].append(thr[0].iloc[i]['time']/row['time'])
            i+=1

    for id in range(1,5):
        fig.add_trace(go.Scatter(
            x=x[id-1],
            y=y[id-1],
            mode='markers+lines',
            name=str(id)+" thread",
            marker_symbol='circle',
            marker=dict(
                # color=randint(1, 500),
                line_width=1,
                size=8,
            ),
            
        ))
    # fig.update_xaxes(type="log")
    # fig.update_yaxes(type="log")
    fig.update_layout(
        title=gen_name,
        xaxis_title="Ilość elementów w tablicy",
        yaxis_title="Przyspieszenie"
    )


# schedule = ['static', 'static,4', 'dynamic', 'dynamic,4', 'dynamic,50', 'guided,4', 'guided']
# sched_dict = {}
# for sched_name in schedule:
#     df = pd.read_csv('results_'+sched_name.replace(',', '_')+'.txt', ";")
#     print(sched_name)
#     print(df)
#     df.columns = ['threads', 'size', 'time', 'nan']
#     sched_dict[sched_name] = [df.loc[df['threads'] == x] for x in range(1,5)]

# time_schedule(sched_dict, schedule, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")

# schedule2 = ['static', 'dynamic', 'guided']
# time_schedule(sched_dict, schedule2, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")
# schedule3 = ['static,4', 'dynamic,4', 'guided,4']
# time_schedule(sched_dict, schedule3, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")
# schedule4 = [ 'dynamic', 'dynamic,4', 'dynamic,50']
# time_schedule(sched_dict, schedule4, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")


# schedule = ['static', 'static,4', 'dynamic', 'dynamic,4', 'dynamic,50', 'guided,4', 'guided']
# for sched_name in schedule:
#     fig = go.Figure()

#     df1 = pd.read_csv('results_'+sched_name.replace(',', '_')+'.txt', ";")
#     df1.columns = ['threads', 'size', 'time', 'nan']
#     print(df1)

#     time_size(df1, fig, 3000000000.0, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla parametru schedule: "+sched_name)
#     fig.show()

schedule = ['static', 'static,4', 'dynamic', 'dynamic,4', 'dynamic,50', 'guided,4', 'guided']
for sched_name in schedule:
    fig = go.Figure()

    df1 = pd.read_csv('results_'+sched_name.replace(',', '_')+'.txt', ";")
    df1.columns = ['threads', 'size', 'time', 'nan']
    print(df1)

    speedup(df1, fig, 3000000000.0, "Przyspieszenie części równoległej programu z zależności od rozmiaru problemu dla parametru schedule: "+sched_name)
    fig.show()

