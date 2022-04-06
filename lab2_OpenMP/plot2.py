from random import randint
import sched
import matplotlib as plt
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

def time_size(df, fig, limit, gen_name, sched_name):
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
        xaxis_title="Ilość elementów w tablicy tablicy",
        yaxis_title="Czas trwania części równoległej [s]"
    )

def time_schedule(sdict, schedules, gen_name):
    
    for thr in range(3,4):
        fig = go.Figure()
        x = {}
        y = {}
        for name in schedules:
            res_list=sdict[name]
            df1 = res_list[thr]
            for idx, row in df1.iterrows():
                if name not in x or name not in y:
                    x[name] = []
                    y[name] = []
                x[name].append(row['size']) 
                y[name].append(row['time'])
        
        
        for name in schedules:
            fig.add_trace(go.Scatter(
                x=x[name],
                y=y[name],
                mode='markers+lines',
                name=name,
                marker_symbol='circle',
                marker=dict(
                    line_width=1,
                    size=8,
                ),
                
            ))
        fig.update_xaxes(type="log")
        fig.update_yaxes(type="log")
        fig.update_layout(
            title=gen_name+str(thr+1),
            xaxis_title="Ilość elementów w tablicy tablicy",
            yaxis_title="Czas trwania części równoległej [s]"
        )
        fig.show()
        print("one")


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


schedule = ['static', 'static,4', 'dynamic', 'dynamic,4', 'dynamic,50', 'guided,4', 'guided']
sched_dict = {}
for sched_name in schedule:
    df = pd.read_csv('results_'+sched_name.replace(',', '_')+'.txt', ";")
    df.columns = ['time', ' ', ' ' ,  ' ',' ',' ',' ',' ',' ', 'threads', 'size', 'nan']
    sched_dict[sched_name] = [df.loc[df['threads'] == x] for x in range(1,5)]

time_schedule(sched_dict, schedule, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")

schedule2 = ['static', 'dynamic', 'guided']
time_schedule(sched_dict, schedule2, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")
schedule3 = ['static,4', 'dynamic,4', 'guided,4']
time_schedule(sched_dict, schedule3, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")
schedule4 = [ 'dynamic', 'dynamic,4', 'dynamic,50']
time_schedule(sched_dict, schedule4, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla liczby wątków: ")


schedule = ['static', 'static,4', 'dynamic', 'dynamic,4', 'dynamic,50', 'guided,4', 'guided']
for sched_name in schedule:
    fig = go.Figure()

    df1 = pd.read_csv('results_'+sched_name.replace(',', '_')+'.txt', ";")
    df1.columns = ['time', ' ', ' ' ,  ' ',' ',' ',' ',' ',' ', 'threads', 'size', 'nan']
    print(df1)

    time_size(df1, fig, 3000000000.0, "Czas trwania części równoległej programu z zależności od rozmiaru problemu dla parametru schedule: "+sched_name)
    fig.show()


