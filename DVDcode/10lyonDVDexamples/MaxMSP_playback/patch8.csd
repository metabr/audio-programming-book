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
ga1 oscil 1.0, 0.160647, isine
ga1 = -0.3 + (0.300000*ga1 + 0.300000)
ga2 oscil 1.0, 0.182989, isine
ga2 = 0 + (1.000000*ga2 + 1.000000)
ga3 oscil 1.0, 0.237064, isine
ga4 oscil ga2, 315.944427, isine
ga4 = -2.806472 + (2.806472*ga4 + 2.806472)
ga5 oscil 1.0, 5.889942, isine
ga5 = -0.018555 + (0.018555*ga5 + 0.018555)
ga6 oscil 0.7*ga2, 317.7913 * (1.0 + ga4+ga5), itriangle
ga7 oscil 1.0, 0.181400, isine
ga7 = -0.3 + (0.300000*ga7 + 0.300000)
ga8 oscil 1.0, 0.225693, isine
ga8 = 0 + (1.000000*ga8 + 1.000000)
ga9 oscil 1.0, 0.061759, isine
ga10 oscil ga8, 397.978516, isine
ga10 = -2.224943 + (2.224943*ga10 + 2.224943)
ga11 oscil 1.0, 4.904380, isine
ga11 = -0.007714 + (0.007714*ga11 + 0.007714)
ga12 oscil 0.7*ga8, 397.2391 * (1.0 + ga10+ga11), isine
ga13 oscil 1.0, 0.194168, isine
ga13 = -0.3 + (0.300000*ga13 + 0.300000)
ga14 oscil 1.0, 0.136819, isine
ga14 = 0 + (1.000000*ga14 + 1.000000)
ga15 oscil 1.0, 0.157796, isine
ga16 oscil ga14, 480.207581, isine
ga16 = -7.836858 + (7.836858*ga16 + 7.836858)
ga17 oscil 1.0, 5.223249, isine
ga17 = -0.010341 + (0.010341*ga17 + 0.010341)
ga18 oscil 0.7*ga14, 476.6869 * (1.0 + ga16+ga17), ipulse
ga19 oscil 1.0, 0.428351, isine
ga19 = -0.3 + (0.300000*ga19 + 0.300000)
ga20 oscil 1.0, 0.139159, isine
ga20 = 0 + (1.000000*ga20 + 1.000000)
ga21 oscil 1.0, 0.298805, isine
ga22 oscil ga20, 113.591713, isine
ga22 = -6.165322 + (6.165322*ga22 + 6.165322)
ga23 oscil 1.0, 5.806407, isine
ga23 = -0.010650 + (0.010650*ga23 + 0.010650)
ga24 oscil 0.7*ga20, 112.7089 * (1.0 + ga22+ga23), ipulse
ga25 oscil 1.0, 0.291957, isine
ga25 = -0.3 + (0.300000*ga25 + 0.300000)
ga26 oscil 1.0, 0.083920, isine
ga26 = 0 + (1.000000*ga26 + 1.000000)
ga27 oscil 1.0, 0.210123, isine
ga28 oscil ga26, 134.505829, isine
ga28 = -7.682796 + (7.682796*ga28 + 7.682796)
ga29 oscil 1.0, 3.529280, isine
ga29 = -0.010023 + (0.010023*ga29 + 0.010023)
ga30 oscil 0.7*ga26, 135.2507 * (1.0 + ga28+ga29), isquare
ga31 oscil 1.0, 0.293982, isine
ga31 = -0.3 + (0.300000*ga31 + 0.300000)
ga32 oscil 1.0, 0.054934, isine
ga32 = 0 + (1.000000*ga32 + 1.000000)
ga33 oscil 1.0, 0.231561, isine
ga34 oscil ga32, 167.559662, isine
ga34 = -5.980732 + (5.980732*ga34 + 5.980732)
ga35 oscil 1.0, 3.800282, isine
ga35 = -0.007414 + (0.007414*ga35 + 0.007414)
ga36 oscil 0.7*ga32, 169.0634 * (1.0 + ga34+ga35), isawtooth
ga37 oscil 1.0, 0.490084, isine
ga37 = -0.3 + (0.300000*ga37 + 0.300000)
ga38 oscil 1.0, 0.280769, isine
ga38 = 0 + (1.000000*ga38 + 1.000000)
ga39 oscil 1.0, 0.088998, isine
ga40 oscil ga38, 469.022797, isine
ga40 = -4.018076 + (4.018076*ga40 + 4.018076)
ga41 oscil 1.0, 2.483877, isine
ga41 = -0.019374 + (0.019374*ga41 + 0.019374)
ga42 oscil 0.7*ga38, 468.1331 * (1.0 + ga40+ga41), ipulse
ga43 oscil 1.0, 0.418829, isine
ga43 = -0.3 + (0.300000*ga43 + 0.300000)
ga44 oscil 1.0, 0.156695, isine
ga44 = 0 + (1.000000*ga44 + 1.000000)
ga45 oscil 1.0, 0.078365, isine
ga46 oscil ga44, 561.009216, isine
ga46 = -4.124260 + (4.124260*ga46 + 4.124260)
ga47 oscil 1.0, 4.393278, isine
ga47 = -0.018140 + (0.018140*ga47 + 0.018140)
ga48 oscil 0.7*ga44, 561.7598 * (1.0 + ga46+ga47), itriangle
ga49 oscil 1.0, 0.236605, isine
ga49 = -0.3 + (0.300000*ga49 + 0.300000)
ga50 oscil 1.0, 0.149523, isine
ga50 = 0 + (1.000000*ga50 + 1.000000)
ga51 oscil 1.0, 0.222122, isine
ga52 oscil ga50, 696.392212, isine
ga52 = -2.163697 + (2.163697*ga52 + 2.163697)
ga53 oscil 1.0, 2.024106, isine
ga53 = -0.016971 + (0.016971*ga53 + 0.016971)
ga54 oscil 0.7*ga50, 702.1997 * (1.0 + ga52+ga53), isine
ga55 oscil 1.0, 0.362909, isine
ga55 = -0.3 + (0.300000*ga55 + 0.300000)
ga56 oscil 1.0, 0.186971, isine
ga56 = 0 + (1.000000*ga56 + 1.000000)
ga57 oscil 1.0, 0.104233, isine
ga58 oscil ga56, 414.236359, isine
ga58 = -5.376803 + (5.376803*ga58 + 5.376803)
ga59 oscil 1.0, 1.848942, isine
ga59 = -0.007808 + (0.007808*ga59 + 0.007808)
ga60 oscil 0.7*ga56, 412.9971 * (1.0 + ga58+ga59), isquare
ga61 oscil 1.0, 0.263990, isine
ga61 = -0.3 + (0.300000*ga61 + 0.300000)
ga62 oscil 1.0, 0.273745, isine
ga62 = 0 + (1.000000*ga62 + 1.000000)
ga63 oscil 1.0, 0.236629, isine
ga64 oscil ga62, 515.726807, isine
ga64 = -4.589262 + (4.589262*ga64 + 4.589262)
ga65 oscil 1.0, 5.881377, isine
ga65 = -0.007807 + (0.007807*ga65 + 0.007807)
ga66 oscil 0.7*ga62, 516.2463 * (1.0 + ga64+ga65), ipulse
ga67 oscil 1.0, 0.366224, isine
ga67 = -0.3 + (0.300000*ga67 + 0.300000)
ga68 oscil 1.0, 0.176844, isine
ga68 = 0 + (1.000000*ga68 + 1.000000)
ga69 oscil 1.0, 0.296051, isine
ga70 oscil ga68, 623.457214, isine
ga70 = -5.377088 + (5.377088*ga70 + 5.377088)
ga71 oscil 1.0, 4.552172, isine
ga71 = -0.008454 + (0.008454*ga71 + 0.008454)
ga72 oscil 0.7*ga68, 619.4956 * (1.0 + ga70+ga71), ipulse
kenv linseg 0,.05,1835.827026,p3-0.1,1835.827026,.05,0
out (ga6)*kenv
kenv linseg 0,.05,1915.070068,p3-0.1,1915.070068,.05,0
out (ga12)*kenv
kenv linseg 0,.05,1110.793457,p3-0.1,1110.793457,.05,0
out (ga18)*kenv
kenv linseg 0,.05,567.828430,p3-0.1,567.828430,.05,0
out (ga24)*kenv
kenv linseg 0,.05,692.345947,p3-0.1,692.345947,.05,0
out (ga30)*kenv
kenv linseg 0,.05,1121.474121,p3-0.1,1121.474121,.05,0
out (ga36)*kenv
kenv linseg 0,.05,1869.275391,p3-0.1,1869.275391,.05,0
out (ga42)*kenv
kenv linseg 0,.05,507.639709,p3-0.1,507.639709,.05,0
out (ga48)*kenv
kenv linseg 0,.05,598.937561,p3-0.1,598.937561,.05,0
out (ga54)*kenv
kenv linseg 0,.05,1526.201538,p3-0.1,1526.201538,.05,0
out (ga60)*kenv
kenv linseg 0,.05,413.504578,p3-0.1,413.504578,.05,0
out (ga66)*kenv
kenv linseg 0,.05,713.381470,p3-0.1,713.381470,.05,0
out (ga72)*kenv
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
