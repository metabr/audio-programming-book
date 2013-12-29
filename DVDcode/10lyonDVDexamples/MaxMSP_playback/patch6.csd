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
ga13 init 0.0
ga14 init 0.0
ga15 init 0.0
ga16 init 0.0
ga17 init 0.0
ga18 init 0.0
ga19 init 0.0
ga20 init 0.0
ga21 init 0.0
ga22 init 0.0
ga23 init 0.0
ga24 init 0.0
ga1 oscil 1.0, 2.108705, isine
ga1 = -0.3 + (0.300000*ga1 + 0.300000)
ga2 oscil 0.7+ga1, 246.4179, itriangle
ga3 oscil 1.0, 0.319828, isine
ga3 = -0.3 + (0.300000*ga3 + 0.300000)
ga4 oscil 0.7+ga3, 308.0224, isine
ga5 oscil 1.0, 1.715374, isine
ga5 = -0.3 + (0.300000*ga5 + 0.300000)
ga6 oscil 0.7+ga5, 369.6269, itriangle
ga7 oscil 1.0, 2.952379, isine
ga7 = -0.3 + (0.300000*ga7 + 0.300000)
ga8 oscil 0.7+ga7, 281.6561, isawtooth
ga9 oscil 1.0, 1.811890, isine
ga9 = -0.3 + (0.300000*ga9 + 0.300000)
ga10 oscil 0.7+ga9, 352.0701, isawtooth
ga11 oscil 1.0, 1.885570, isine
ga11 = -0.3 + (0.300000*ga11 + 0.300000)
ga12 oscil 0.7+ga11, 422.4841, ipulse
ga13 oscil 1.0, 1.091745, isine
ga13 = -0.3 + (0.300000*ga13 + 0.300000)
ga14 oscil 0.7+ga13, 481.7315, isquare
ga15 oscil 1.0, 2.978362, isine
ga15 = -0.3 + (0.300000*ga15 + 0.300000)
ga16 oscil 0.7+ga15, 578.0778, ipulse
ga17 oscil 1.0, 1.249751, isine
ga17 = -0.3 + (0.300000*ga17 + 0.300000)
ga18 oscil 0.7+ga17, 722.5973, itriangle
ga19 oscil 1.0, 0.875829, isine
ga19 = -0.3 + (0.300000*ga19 + 0.300000)
ga20 oscil 0.7+ga19, 127.6070, isawtooth
ga21 oscil 1.0, 1.566285, isine
ga21 = -0.3 + (0.300000*ga21 + 0.300000)
ga22 oscil 0.7+ga21, 153.1284, ipulse
ga23 oscil 1.0, 2.190656, isine
ga23 = -0.3 + (0.300000*ga23 + 0.300000)
ga24 oscil 0.7+ga23, 191.4105, itriangle
kenv linseg 0,.05,921.358948,p3-0.1,921.358948,.05,0
out (ga2)*kenv
kenv linseg 0,.05,801.720154,p3-0.1,801.720154,.05,0
out (ga4)*kenv
kenv linseg 0,.05,763.270264,p3-0.1,763.270264,.05,0
out (ga6)*kenv
kenv linseg 0,.05,875.434387,p3-0.1,875.434387,.05,0
out (ga8)*kenv
kenv linseg 0,.05,1192.874634,p3-0.1,1192.874634,.05,0
out (ga10)*kenv
kenv linseg 0,.05,492.151001,p3-0.1,492.151001,.05,0
out (ga12)*kenv
kenv linseg 0,.05,1541.282837,p3-0.1,1541.282837,.05,0
out (ga14)*kenv
kenv linseg 0,.05,724.879517,p3-0.1,724.879517,.05,0
out (ga16)*kenv
kenv linseg 0,.05,1842.759888,p3-0.1,1842.759888,.05,0
out (ga18)*kenv
kenv linseg 0,.05,1660.501709,p3-0.1,1660.501709,.05,0
out (ga20)*kenv
kenv linseg 0,.05,1970.409302,p3-0.1,1970.409302,.05,0
out (ga22)*kenv
kenv linseg 0,.05,1666.910156,p3-0.1,1666.910156,.05,0
out (ga24)*kenv
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
