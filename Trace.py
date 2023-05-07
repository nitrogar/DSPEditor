import numpy as np
# fit after alignment problem
class Trace:
    verbose = False
    def __init__(self, name, training_label, trace) -> None:
        self.name = name
        self.training_label = training_label
        self.trace = trace
        self.verbose = False
        self.log(f"setting Trace to name {self.name} label: {self.training_label} trace: {len(self.trace)}")
        self.log(f"against {name} label:{training_label} trace:{len(trace)}")
    #def __iter__(self):
    #    pass
    #    for s in self.trace:
    #        yield s
    #        
    def __str__(self):
        return f"[{[i for i in self]}]"
    def __getitem__(self,slice):
        s = slice.indices(len(self.trace))
        self.log(f"inside slicer label: {self.training_label}")
        return Trace(self.name,self.training_label,self.trace[s[0]:s[1]])
    def plot(self,backend='matplotlib'):
        if backend == 'matplotlib':
            import matplotlib.pyplot as plt
            plt.plot(self.trace, label=self.name)
            
        elif backend == 'plotly' :
            import plotly.express as px
            fig = px.line(self.trace, label=self.name)
            fig.show()
            pass
    
    def c_cross_correlation(a,b):
        return np.correlate(a,b,"full")
    
    def cross_correlation(self,other):
        return self.c_cross_correlation(self.trace,other.trace)
    
    def g_cross_correlation(self,other):
        pass
    
    def c_optimal_alignment_shift(a,b):
        xcor = c_cross_correlaion(a,b)
        shift = np.argmax(xcor)
        return shift
    
    def optimal_alignment_shift(self,other):
        return c_optimal_alignment_shift(self.trace,other.trace)
    
    def shift_trace(self,n):
        if n >= 0:
            return Trace(self.name,self.training_label,[0] * n + self.trace)
        else:
            return Trace(self.name,self.training_label,self.trace + [0] * n)
    def cut_trace(self,n) :
        if n < len(self.trace):
            self.log(f"before creating new trace {self.training_label}")
            t = Trace(self.name,self.training_label,self.trace[n:])
            self.log(f"inside cut trace new : {t.training_label} me:{self.training_label}")
            return t
        else:
            raise Exception(f"Trying to cut trace with no enough samples trace size: {len(self.trace)} cut: {n}")
    def align_with(self,other):
        shift = self.optimal_alignment_shift(other)
        return shift_trace(shift)
    
    def differtial(self,other):
        return self.c_differtial(self.trace,other.trace)
    def c_differtial(a,b):
        return a - b
    def g_differtial(self,other):
        pass
    def apply(self,operator,other=None):
        pass
    def export_to_inspector(self):
        pass
    def pattern_recognition(self,pattern):
        pass
    def pattern_extruction(self,pattern):
        pass
    def pattern_detection(self):
        pass
    def get_raw_data(self):
        return self.trace
    def value_at(self,n):
        print(f"trace {self.name} value at {n} is {self.trace[n]}")
        return self.trace[n]

    def log(self,msg):
        if self.verbose:
            print(msg)
    def get_training_data(self):
        return self.trace,self.training_label
    def size(self):
        return len(self.trace)
