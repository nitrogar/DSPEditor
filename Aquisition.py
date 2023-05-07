from TraceGroup import TraceGroup
import os
from Trace import Trace
from CWSope import CWSope
class Tool:
  def __init__(self):
        pass
  def setup_target(self,target_name):
        raise Exception("using Dummy setupt target")
  def setup_programmer(self, tool="cw",options=None):
    raise Exception("using Dummy setupt programmer")
  def program_target(self,firmware_path,options=None):
    raise Exception("using Dummy program target")
  def capture(self,message,options=None):
    raise Exception("using Dummy capture")
  def set_num_samples(self,n):
    raise Exception("using Dummy set_num_samples")
  def set_frq(self,frq):
    raise Exception("using Dummy set_frq")
  def close(self):
    raise Exception("using Dummy close")
 

class Aquisition:
    
    def __init__(self,tool_name,ntraces) -> None:
        self.tool_name = tool_name
        self.ntraces = ntraces
 
        self.tool_instance = Tool()
        self.programmer_instance = None
        self.nsamples = 0
        self.sample_rate = 0
        self.freq = 0
        self.current = None
        self.old = []
        self.capture_options = {}
        self.run_before = False    
        self.verbose = False 
    def __iter__(self):
        pass
    def __next__(self):
        pass
    def set_capture_options(self,options):
        self.capture_options = options
    def __str__(self):
        return f"""
        tool_name : {self.tool_name}
        tool_instance : {self.tool_instance}
        programmer_instance: {self.programmer_instance}
        nsamples:   {self.nsamples}
        sample_rate: {self.sample_rate}
        freq:   {self.freq}
        ntraces: {self.ntraces}
        currnet: {self.current}
        old:    {len(self.old)}
        """

    def connect_tool(self):
        if self.tool_name == "cw" :
            self.tool_instance = CWSope()
    def is_tool_connected(self):
        return self.tool_instance != None
    def start(self,n):
        self.ntraces = n
        if self.run_before == False:
            self.tool_instance.setup_target("simpleserial")
            self.tool_instance.set_num_samples(self.nsamples)

        self.run_before = True
        if self.current != None:
            self.old.append(self.current)
        
        self.current = TraceGroup(f"origin_{self.ntraces}")
        self.log(f"[] capturing {self.ntraces} ...")

        i = 0
        for _ in range(self.ntraces):
            msg = self.generate_random_bytes(32)
            p = self.generate_random_bytes(32)
            option = {}
            option['cmd']  = 'l'
            option['scmd'] = 0
            option['return_value'] = msg
            option['response_size'] = 32

            self.log(f"[{_}] capturing {msg} ...")
            raw_trace = self.tool_instance.capture(msg + p,options=option)
            self.log(f"raw trake: {raw_trace}")
            trace = Trace(f"trace of {msg}",msg,raw_trace)
            self.current.add_trace(trace)
            print(f"[{i}/{n}] capturing progress {'.'*(i%5)}\r",end='')
            i = i + 1
        return self.current 
        
    def get_last_aquired(self):
         return self.current
    def clear_all(self):
        self.current = None
        self.old = []
    def set_number_of_samples(self,n):
        self.nsamples = n
    def generate_random_bytes(self,n):
        return bytearray(os.urandom(n))
    def program(self,firmware_path):
        if self.tool_name == "cw":
            self.tool_instance.setup_programmer("cw","STM32")
            self.tool_instance.program_target(firmware_path)
    
    def close_tool(self):
        self.tool_instance.close()
    def log(self,msg):
        if self.verbose:
            print(msg)
