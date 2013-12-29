<CsoundSynthesizer>
<CsOptions>
-odac -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 1

/* you may need to edit the CsOptions 
in order to run this example. More 
information on running Csound can be 
found in the Csound reference manual. */

instr 10
;dummy instrument
endin

instr 20
kamp expon p4, p3, 0.001
a1 oscil kamp, p5, 1
out a1
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
i10 0 1000


</CsScore>
</CsoundSynthesizer>
