<CsoundSynthesizer>
<CsOptions>
-+rtaudio=alsa -odac ;for linux
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64	
nchnls = 1
       

instr 1
/* host chnnels */
kfreq chnget "freq"
kdens chnget "dens"
kaoff chnget "ampoff"
kpoff chnget "pitchoff"
kgdur chnget "graindur"

;comment the following if you wish not to use real-time intput
ain  inch  1

;replace banjo.wav with a sound file if you wish not to use real time intput
;ain soundin "/home/rory/Documents/banjo.wav"
awp  phasor sr/ftlen(1)
awin tablei awp, 2, 1
     tablew (ain*awin), awp, 1, 1 
insnd = 1
;change this from random...
kbasfrq randh (sr/2) / ftlen(insnd), 2   ; Use original sample rate of insnd file   
kamp   = 1
kpitch = ((kfreq)*44100)/ ftlen(insnd);
imaxgdur =  1 
ar  grain kamp, kpitch, kdens+1, kaoff+0.01, kpoff+0.01, kgdur+0.01, insnd, 3, imaxgdur
out ar 
endin   

</CsInstruments>
<CsScore>
f1 0 131072 7 0 131072 0
f2 0 131072 7 0 36 1 131000 1 36 0
f3 0 1024 -20 1
i1 0 3600  
</CsScore>
</CsoundSynthesizer>
