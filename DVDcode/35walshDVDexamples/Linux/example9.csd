<CsoundSynthesizer>
<CsOptions>
-+rtaudio=alsa -odac ;for linux
</CsOptions>
<CsInstruments>
sr = 44100
kr = 4410
ksmps = 10
nchnls = 2

/* you may need to edit the CsOptions 
in order to run this example. More 
information on running Csound can be 
found in the Csound reference manual. */

instr 10
kin chnget "loop"
kchn changed kin
if(kchn==1) then
event "i", 20, 0, 40, kin
endif
endin

instr 20
kenv oscil 6000, 1/2, 2 
knote oscil 1, 1/8, p4
a1 oscil kenv, cpspch(knote+6), 1
acombL comb a1, 2, 1.5 
acombR comb a1, 2, 0.4
outs acombL, acombR
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1 0 1 0 1
f2 0 1024 5 0.001 100 1 924 0.001  
f3 0 8 2 .00 .05 -1   .05 .04 .02 .04 1
f4 0 8 2 .05 .02 0.06 .05 -1 .02 1 .04
f5 0 8 2 .00 1 0.05 .05 .04 .02 .04 .01
f6 0 8 2 .02 .02 0.06 .05 .04 .02 -1 1
f7 0 8 2 .04 .05 1 .05 .04 .02 .04 1
i10 0 100
</CsScore>
</CsoundSynthesizer>
