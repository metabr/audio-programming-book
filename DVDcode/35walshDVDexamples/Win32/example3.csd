<CsoundSynthesizer>
<CsOptions>
;-odevaudio -b10 -g ;for windows
;-+rtaudio=alsa -odac ;for linux
-odevaudio -b1024 -d
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
kval invalue "amp"
kamp expon i(kval), p3, 0.001
a1 oscil kamp, P4, 1
out a1
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
i1 0 10 220
i1 2 10 310
i1 4 10 420
i1 6 10 540
</CsScore>
</CsoundSynthesizer>
