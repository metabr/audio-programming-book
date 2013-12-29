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
ga4 init 0.0
ga5 init 0.0
ga6 init 0.0
ga7 init 0.0
ga8 init 0.0
ga9 init 0.0
ga10 init 0.0
ga11 init 0.0
ga12 init 0.0
ga1 oscil 1.0, 153.4604, ipulse
ga2 oscil 1.0, 191.8255, isquare
ga3 oscil 1.0, 230.1906, isquare
ga4 oscil 1.0, 171.8857, ipulse
ga5 oscil 1.0, 214.8571, itriangle
ga6 oscil 1.0, 257.8286, ipulse
ga7 oscil 1.0, 627.6076, ipulse
ga8 oscil 1.0, 753.1292, ipulse
ga9 oscil 1.0, 941.4114, itriangle
ga10 oscil 1.0, 738.3207, isquare
ga11 oscil 1.0, 922.9009, isquare
ga12 oscil 1.0, 1107.4810, isquare
kenv linseg 0,.05,2000.000000,p3-0.1,2000.000000,.05,0
out (ga1+ga2+ga3)*kenv
kenv linseg 0,.05,2000.000000,p3-0.1,2000.000000,.05,0
out (ga4+ga5+ga6)*kenv
kenv linseg 0,.05,2000.000000,p3-0.1,2000.000000,.05,0
out (ga7+ga8+ga9)*kenv
kenv linseg 0,.05,2000.000000,p3-0.1,2000.000000,.05,0
out (ga10+ga11+ga12)*kenv
	endin

</CsInstruments>
<CsScore>

f1 0 8192 10 1 ; sine
f2 0 8192 10 1 0 .111 0 .04 0 .02 0 ; triangle
f3 0 8192 10 1 .5 .333 .25 .2 .166 .142 .125 ; sawtooth
f4 0 8192 10 1 0 .333 0 .2 0 .142 0 .111; square
f5 0 8192 10 1 1 1 1 1 1 1 1 1 1 1 1 1; pulse

i1 0 30.000000

</CsScore>
</CsoundSynthesizer>
