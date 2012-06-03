# -*- coding:utf-8 -*-

import file_mutex
import threading
import time

class TestMutex(threading.Thread) :
    def __init__(self) :
        self.f = file_mutex.File("Hello")
        self.g = file_mutex.File("Hello")
        
        threading.Thread.__init__(self, target=self.ecrireF)
        
        self.launch()
        
    def launch(self) :
        self.start()
        
        for i in range(10,20) :
            self.g.write(str(i))
            time.sleep(0.01)
            
    def ecrireF(self) :
        for i in range(10) :
            self.f.write(str(i))
            time.sleep(0.01)
            
    

        