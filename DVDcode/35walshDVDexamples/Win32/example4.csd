<CsoundSynthesizer>
<CsOptions>
-odevaudio -b1024 -d
;-+rtaudio=alsa -odac ;for linux
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
a1 oscil 32000, kfreq, 1
out a1
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
i1 0 100
</CsScore>
</CsoundSynthesizer>
