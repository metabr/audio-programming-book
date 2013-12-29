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
ga25 init 0.0
ga26 init 0.0
ga27 init 0.0
ga28 init 0.0
ga29 init 0.0
ga30 init 0.0
ga31 init 0.0
ga32 init 0.0
ga33 init 0.0
ga34 init 0.0
ga35 init 0.0
ga36 init 0.0
ga37 init 0.0
ga38 init 0.0
ga39 init 0.0
ga40 init 0.0
ga41 init 0.0
ga42 init 0.0
ga43 init 0.0
ga44 init 0.0
ga45 init 0.0
ga46 init 0.0
ga47 init 0.0
ga48 init 0.0
ga49 init 0.0
ga50 init 0.0
ga51 init 0.0
ga52 init 0.0
ga53 init 0.0
ga54 init 0.0
ga55 init 0.0
ga56 init 0.0
ga57 init 0.0
ga58 init 0.0
ga59 init 0.0
ga60 init 0.0
ga61 init 0.0
ga62 init 0.0
ga63 init 0.0
ga64 init 0.0
ga65 init 0.0
ga66 init 0.0
ga67 init 0.0
ga68 init 0.0
ga69 init 0.0
ga70 init 0.0
ga71 init 0.0
ga72 init 0.0
ga1 oscil 1.0, 0.450191, isine
ga1 = -0.3 + (0.300000*ga1 + 0.300000)
ga2 oscil 1.0, 0.066331, isine
ga2 = 0.2 + (0.650000*ga2 + 0.650000)
ga3 oscil 1.0, 0.190647, isine
ga4 oscil ga2, 298.282715, isine
ga4 = -4.817634 + (4.817634*ga4 + 4.817634)
ga5 oscil 1.0, 4.582787, isine
ga5 = -0.013512 + (0.013512*ga5 + 0.013512)
ga6 oscil 0.7*ga2, 296.6284 * (1.0 + ga4+ga5), isquare
ga7 oscil 1.0, 0.374607, isine
ga7 = -0.3 + (0.300000*ga7 + 0.300000)
ga8 oscil 1.0, 0.021939, isine
ga8 = 0.2 + (0.650000*ga8 + 0.650000)
ga9 oscil 1.0, 0.225882, isine
ga10 oscil ga8, 368.624908, isine
ga10 = -5.747722 + (5.747722*ga10 + 5.747722)
ga11 oscil 1.0, 3.477172, isine
ga11 = -0.015295 + (0.015295*ga11 + 0.015295)
ga12 oscil 0.7*ga8, 370.7855 * (1.0 + ga10+ga11), isawtooth
ga13 oscil 1.0, 0.472850, isine
ga13 = -0.3 + (0.300000*ga13 + 0.300000)
ga14 oscil 1.0, 0.011923, isine
ga14 = 0.2 + (0.650000*ga14 + 0.650000)
ga15 oscil 1.0, 0.161641, isine
ga16 oscil ga14, 440.989960, isine
ga16 = -7.041698 + (7.041698*ga16 + 7.041698)
ga17 oscil 1.0, 1.129726, isine
ga17 = -0.016756 + (0.016756*ga17 + 0.016756)
ga18 oscil 0.7*ga14, 444.9426 * (1.0 + ga16+ga17), isquare
ga19 oscil 1.0, 0.284931, isine
ga19 = -0.3 + (0.300000*ga19 + 0.300000)
ga20 oscil 1.0, 0.035391, isine
ga20 = 0.2 + (0.650000*ga20 + 0.650000)
ga21 oscil 1.0, 0.271533, isine
ga22 oscil ga20, 145.224457, isine
ga22 = -2.489732 + (2.489732*ga22 + 2.489732)
ga23 oscil 1.0, 0.742829, isine
ga23 = -0.015213 + (0.015213*ga23 + 0.015213)
ga24 oscil 0.7*ga20, 144.3065 * (1.0 + ga22+ga23), ipulse
ga25 oscil 1.0, 0.389090, isine
ga25 = -0.3 + (0.300000*ga25 + 0.300000)
ga26 oscil 1.0, 0.027226, isine
ga26 = 0.2 + (0.650000*ga26 + 0.650000)
ga27 oscil 1.0, 0.108419, isine
ga28 oscil ga26, 174.888306, isine
ga28 = -4.167461 + (4.167461*ga28 + 4.167461)
ga29 oscil 1.0, 4.337032, isine
ga29 = -0.016187 + (0.016187*ga29 + 0.016187)
ga30 oscil 0.7*ga26, 173.1679 * (1.0 + ga28+ga29), isine
ga31 oscil 1.0, 0.326363, isine
ga31 = -0.3 + (0.300000*ga31 + 0.300000)
ga32 oscil 1.0, 0.011095, isine
ga32 = 0.2 + (0.650000*ga32 + 0.650000)
ga33 oscil 1.0, 0.053937, isine
ga34 oscil ga32, 217.412872, isine
ga34 = -1.799354 + (1.799354*ga34 + 1.799354)
ga35 oscil 1.0, 1.759932, isine
ga35 = -0.018556 + (0.018556*ga35 + 0.018556)
ga36 oscil 0.7*ga32, 216.4598 * (1.0 + ga34+ga35), isquare
ga37 oscil 1.0, 0.215014, isine
ga37 = -0.3 + (0.300000*ga37 + 0.300000)
ga38 oscil 1.0, 0.034884, isine
ga38 = 0.2 + (0.650000*ga38 + 0.650000)
ga39 oscil 1.0, 0.297873, isine
ga40 oscil ga38, 195.925858, isine
ga40 = -6.418305 + (6.418305*ga40 + 6.418305)
ga41 oscil 1.0, 3.839316, isine
ga41 = -0.010559 + (0.010559*ga41 + 0.010559)
ga42 oscil 0.7*ga38, 195.9067 * (1.0 + ga40+ga41), isawtooth
ga43 oscil 1.0, 0.290294, isine
ga43 = -0.3 + (0.300000*ga43 + 0.300000)
ga44 oscil 1.0, 0.040192, isine
ga44 = 0.2 + (0.650000*ga44 + 0.650000)
ga45 oscil 1.0, 0.299103, isine
ga46 oscil ga44, 235.816971, isine
ga46 = -2.877772 + (2.877772*ga46 + 2.877772)
ga47 oscil 1.0, 2.225852, isine
ga47 = -0.017083 + (0.017083*ga47 + 0.017083)
ga48 oscil 0.7*ga44, 235.0880 * (1.0 + ga46+ga47), isine
ga49 oscil 1.0, 0.368126, isine
ga49 = -0.3 + (0.300000*ga49 + 0.300000)
ga50 oscil 1.0, 0.024695, isine
ga50 = 0.2 + (0.650000*ga50 + 0.650000)
ga51 oscil 1.0, 0.039882, isine
ga52 oscil ga50, 295.149841, isine
ga52 = -7.092537 + (7.092537*ga52 + 7.092537)
ga53 oscil 1.0, 3.795318, isine
ga53 = -0.005237 + (0.005237*ga53 + 0.005237)
ga54 oscil 0.7*ga50, 293.8600 * (1.0 + ga52+ga53), isine
ga55 oscil 1.0, 0.190200, isine
ga55 = -0.3 + (0.300000*ga55 + 0.300000)
ga56 oscil 1.0, 0.044732, isine
ga56 = 0.2 + (0.650000*ga56 + 0.650000)
ga57 oscil 1.0, 0.151171, isine
ga58 oscil ga56, 278.612366, isine
ga58 = -6.471067 + (6.471067*ga58 + 6.471067)
ga59 oscil 1.0, 2.155722, isine
ga59 = -0.019576 + (0.019576*ga59 + 0.019576)
ga60 oscil 0.7*ga56, 277.2578 * (1.0 + ga58+ga59), isquare
ga61 oscil 1.0, 0.378102, isine
ga61 = -0.3 + (0.300000*ga61 + 0.300000)
ga62 oscil 1.0, 0.022142, isine
ga62 = 0.2 + (0.650000*ga62 + 0.650000)
ga63 oscil 1.0, 0.053968, isine
ga64 oscil ga62, 335.012756, isine
ga64 = -1.218001 + (1.218001*ga64 + 1.218001)
ga65 oscil 1.0, 4.629260, isine
ga65 = -0.018329 + (0.018329*ga65 + 0.018329)
ga66 oscil 0.7*ga62, 332.7094 * (1.0 + ga64+ga65), isine
ga67 oscil 1.0, 0.154669, isine
ga67 = -0.3 + (0.300000*ga67 + 0.300000)
ga68 oscil 1.0, 0.050645, isine
ga68 = 0.2 + (0.650000*ga68 + 0.650000)
ga69 oscil 1.0, 0.062852, isine
ga70 oscil ga68, 420.024872, isine
ga70 = -6.796701 + (6.796701*ga70 + 6.796701)
ga71 oscil 1.0, 3.574656, isine
ga71 = -0.006174 + (0.006174*ga71 + 0.006174)
ga72 oscil 0.7*ga68, 415.8867 * (1.0 + ga70+ga71), isine
kenv linseg 0,.05,1475.813599,p3-0.1,1475.813599,.05,0
out (ga6)*kenv
kenv linseg 0,.05,1740.872192,p3-0.1,1740.872192,.05,0
out (ga12)*kenv
kenv linseg 0,.05,1310.930908,p3-0.1,1310.930908,.05,0
out (ga18)*kenv
kenv linseg 0,.05,473.765686,p3-0.1,473.765686,.05,0
out (ga24)*kenv
kenv linseg 0,.05,450.437836,p3-0.1,450.437836,.05,0
out (ga30)*kenv
kenv linseg 0,.05,1265.835205,p3-0.1,1265.835205,.05,0
out (ga36)*kenv
kenv linseg 0,.05,906.511169,p3-0.1,906.511169,.05,0
out (ga42)*kenv
kenv linseg 0,.05,1748.133301,p3-0.1,1748.133301,.05,0
out (ga48)*kenv
kenv linseg 0,.05,1034.266968,p3-0.1,1034.266968,.05,0
out (ga54)*kenv
kenv linseg 0,.05,1314.450195,p3-0.1,1314.450195,.05,0
out (ga60)*kenv
kenv linseg 0,.05,718.824524,p3-0.1,718.824524,.05,0
out (ga66)*kenv
kenv linseg 0,.05,1648.739258,p3-0.1,1648.739258,.05,0
out (ga72)*kenv
	endin

</CsInstruments>
<CsScore>

f1 0 8192 10 1 ; sine
f2 0 8192 10 1 0 .111 0 .04 0 .02 0 ; triangle
f3 0 8192 10 1 .5 .333 .25 .2 .166 .142 .125 ; sawtooth
f4 0 8192 10 1 0 .333 0 .2 0 .142 0 .111; square
f5 0 8192 10 1 1 1 1 1 1 1 1 1 1 1 1 1; pulse

i1 0 60.000000

</CsScore>
</CsoundSynthesizer>
