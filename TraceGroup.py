import numpy as np
import pickle 
class TraceGroup:
    meta = {}
    plot_backend = "plotly"
    def __init__(self,name) -> None:
        self.traces = []
        self.name = name 
        self.training_label = ""
        self.verbose = False
    def __iter__(self):
        for s in self.traces:
            yield s
   
    def __getitem__(self,sl):

        tg = TraceGroup(f"{self.name} - selceting {sl}")
        if isinstance(sl,int):
            self.log(f"inside indexing label: {self.training_label}")
            tg.add_trace(self.traces[sl])
        elif isinstance(sl,slice):
            s = sl.indices(len(self.trace))
            self.log(f"inside slicer label: {self.training_label}")
            for t in self.traces[s[0]:s[1]]:
                tg.add_trace(t)

        return tg
    def __str__(self) -> str:
        return f"[{[i for i in self]}]"
    def plot(self) -> None:
        if self.plot_backend == "plotly":
            import plotly.graph_objects as go 
            fig = go.Figure()
            for i in range(len(self.traces)):
                t = self.traces[i]
                fig.add_trace(go.Line(y=t.get_raw_data()))
            fig.show()
    def apply_col(self,operator,other=None):
            pass
    def apply_row(self,operator,other=None):
        res = TraceGroup(f"{self.name} - apply_row") 
        for i in range(len(self.traces)):
            r = self.traces[i]
            self.log(f"inside apply row [{i}]{r.training_label}")
            x = operator(r)
            if isinstance(x,list):
                for k in x:
                    self.log(f"adding trace with training label {k.training_label}")
                    res.add_trace(k)
            else: 
                res.add_trace(x)
        self.log("Done aplly row")
        return res

    def extruct_col(self,start,end):
        tg = TraceGroup()
        for t in self.traces:
            tg.add_trace(t[start:end])
    def cut_all(self,n):
        tg = TraceGroup(f"{self.name} - cut by {n}")
        self.log("inside cutt_all .......")
        for i  in range(len(self.traces)):
            self.log(f"cutting {i} with label {self.traces[i].training_label}")
            x = self.traces[i].cut_trace(n)
            self.log(f"after cutting label:{x.training_label}")
            tg.add_trace(x)

        return tg
    def add_trace(self,trace):
        self.traces.append(trace) 
    def find_name(self,name):
        return filter(lambda x: x.name == name, self.traces)
                
    def remove_name(self,name):
        pass
    def remove_trace(self,trace):
        pass
    def align_traces(self,ref_trace):
        new_group = TraceGroup()
        for t in self.traces:
            new_group.add_trace(t.align_with(ref_trace))
       # mybe need fitting after alignment 
        new_group.fit_traces()
        return new_group
    
    def align_with(self,ref_trace_id):
        new_group = TraceGroup(f"{self.name} - alighn with ref {ref_trace_id}")
        ref_trace = self.traces[ref_trace_id]
        return self.align_traces(ref_trace_id)       
    
    def fit_traces(self):
        pass

    def value_at(self,n):
        x = []
        for i in range(len(self.traces)):
            print(f"{i}")
            x.append(self.traces[i].value_at(n))

        return x
    def log(self,msg):
        if self.verbose:
            print(msg)
    def get_training_data(self):
            train_x = []
            train_y = []
            for i in range(len(self.traces)):
                self.log(f"getting trainig data of {self.traces[i].training_label}")
                x,y = self.traces[i].get_training_data()
                train_x.append(x)
                train_y.append(y)
           
    
            size = len(train_x[0])
            number_fo_traces = len(train_x)
            nx_train = np.zeros((number_fo_traces,size), dtype=np.float64)
            ny_train = np.zeros((number_fo_traces,1), dtype=np.int64)
            for i in range(number_fo_traces):
                    nx_train[i] = train_x[i]
                    ny_train[i] = train_y[i]
    
            return nx_train,ny_train
    def size(self):
        return len(self.traces)
    def save(self):
        f = open(self.name,'wb')
        pickle.dump(self,f)
    def load(self):
        f = open(self.name,'rb')
        o = pickle.load(f)
        self.traces = o.traces
        self.name = o.name 
        self.training_label = o.training_label 


