<CsoundSynthesizer>

sr = 44100
kr = 4410
ksmps = 10
nchnls = 1

<CsInstruments>

	instr 1
isine = 1
itriangle = 2
isawtooth = 3
isquare = 4
ipulse = 5
ga1 init 0.0
ga2 init 0.0
ga3 init 0.0
ga1 oscil 1.0, 440, itriangle
ga1 = 1 + (-0.500000*ga1 + -0.500000)
ga2 oscil 1.0, 550, itriangle
ga2 = 1 + (-0.500000*ga2 + -0.500000)
ga3 oscil 1.0, 660, itriangle
ga3 = 1 + (-0.500000*ga3 + -0.500000)
kenv linseg 0,.05,3000.000000,p3-0.1,3000.000000,.05,0
out (ga1+ga2+ga3)*kenv
	endin

</CsInstruments>
<CsScore>

f1 0 8192 10 1 ; sine
f2 0 8192 10 1 0 .111 0 .04 0 .02 0 ; triangle
f3 0 8192 10 1 .5 .333 .25 .2 .166 .142 .125 ; sawtooth
f4 0 8192 10 1 0 .333 0 .2 0 .142 0 .111; square
f5 0 8192 10 1 1 1 1 1 1 1 1 1 1 1 1 1; pulse

i1 0 10.000000

</CsScore>
</CsoundSynthesizer>
