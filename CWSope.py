import chipwhisperer as cw
class CWSope:
  target_name = ""

  def __init__(self):
     self.cw = cw
     self.scope = self.cw.scope()
     self.scope.default_setup()
     target_type = self.cw.targets.SimpleSerial2
     self.target = self.cw.target(self.scope, target_type)
     self.prog = self.cw.programmers.STM32FProgrammer
  def setup_target(self,target_name):
    if target_name == "simpleserial":
      self.target_name = target_name
      target_type = self.cw.targets.SimpleSerial2
      self.target = self.cw.target(self.scope, target_type)
   
  def setup_programmer(self, tool="cw",options=None):
    if tool == "cw":
       if options == "STM32":
          self.prog_tool = "cw"
          self.prog = self.cw.programmers.STM32FProgrammer
  def program_target(self,firmware_path,options=None):
    if self.prog_tool == "cw" :
      self.cw.program_target(self.scope,self.prog,firmware_path)
  
  def capture(self,message,options=None):
    if self.target_name == "simpleserial":
      self.scope.arm()
      self.target.send_cmd(options['cmd'], options['scmd'],message)
      ret = self.scope.capture()
      if ret:
          print("target time out!")

      res = self.target.simpleserial_read('r',options['response_size'])
      trace = self.scope.get_last_trace()
      try: 
        assert res == options['return_value']
      
      except:
                print(f"{options['return_value']} != {res}")
                print(f"Message is {message}")
    return trace
      
  def set_num_samples(self,n):
    self.scope.adc.samples = n
  def set_frq(self,frq):
    pass
  def close(self):
    self.scope.dis()
    
