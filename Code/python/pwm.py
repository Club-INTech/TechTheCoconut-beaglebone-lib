# -*- coding:utf-8 -*-

from File import File

class PWM :
    def __init__(self, frequence_ns) :
        self.frequence_ns = frequence_ns
        
        self.name_file          = "/sys/class/pwm/ehrpwm.2:0"
        self.request_file       = File(self.name_file + "/request")
        self.run_file           = File(self.name_file + "/run")
        self.period_freq_file   = File(self.name_file + "/period_freq")
        self.period_ns_file     = File(self.name_file + "/period_ns")
        self.duty_percent_file  = File(self.name_file + "/duty_percent")
        
        self.request()
        
    # Récuperer le device
    def request(self) :
        self.request_file.write("1")
      
    # Rendre le device
    def free(self) :
        self.request_file.write("0")
        
    # Lancer le PWM
    def start(self) :
        self.run_file.write("1")
    
    # Arrêter le PWM
    def stop(self) :
        self.run_file.write("0")
        
    def setPeriode_byFreq(self, period_freq) :
        self.period_freq_file.write(str(period_freq))
        
    def setPeriode_byNs(self, period_ns) :
        self.period_ns_file.write(str(period_ns))
    
    def setDutyPercent(self, duty_percent) :
        self.duty_percent_file.write(str(duty_percent))