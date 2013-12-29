<CsoundSynthesizer>
<CsOptions>
-odevaudio -b1024 -d ;for windows
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64	
nchnls = 1

/* you may need to edit the CsOptions 
in order to run this example. More 
information on running Csound can be 
found in the Csound reference manual. */

instr 1
kfreq chnget "pitch"
kamp expon 32000, p3, 0.001
a1 oscil kamp, kfreq, 1
out a1
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
i1 0 10 
i1 2 10 
i1 4 10 
i1 6 10 
</CsScore>
</CsoundSynthesizer>
