sr = 44100  
kr = 4410  
ksmps = 10  
nchnls = 1  
instr 1  
k1 chnget "freq" 
a1 oscil 10000, k1+100, 1  
out a1  
endin   
