<CsoundSynthesizer>
<CsOptions>
-+rtaudio=alsa -odac ;for linux
</CsOptions>
<CsInstruments>

/* you may need to edit the CsOptions 
in order to run this example. More 
information on running Csound can be 
found in the Csound reference manual. */

sr=44100
kr=4410
ksmps=10
nchnls=1

instr 1 
;//frequency from 4th p-field
kfreq line p4, p3, 0
idetfactor = 10  
/* amplitude envelope */
iamp = 2000
/* oscillators */
a1 oscili iamp, kfreq, 1
a2 oscili iamp, kfreq+(idetfactor*1/10), 1
a3 oscili iamp, kfreq+(idetfactor*2/10), 1
a4 oscili iamp, kfreq+(idetfactor*3/10), 1
a5 oscili iamp, kfreq+(idetfactor*4/10), 1 
/* sum outputs together */   
out a1+a2+a3+a4+a5
endin

</CsInstruments>  
<CsScore>
;//function table with 5 sinewave components
f1 0 1024 10 1 1 1 1 1
      //;p4
i1 0 15 350
i1 2 300 160  
i1 5 30 660     
i1 8 30 860  
i1 10 3 260 
i1 11 3 660 
</CsScore>
</CsoundSynthesizer>
