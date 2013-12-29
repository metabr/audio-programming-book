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
ga1 oscil 1.0, 0.166313, isine
ga1 = 0.5 + (0.250000*ga1 + 0.250000)
ga2 oscil 1.0, 0.029713, isine
ga2 = 0.2 + (0.650000*ga2 + 0.650000)
ga3 oscil 1.0, 0.035727, isine
ga3 = 0 + (0.500000*ga3 + 0.500000)
ga4 oscil ga2, 97.587006, isine
ga4 = -6.112802 + (6.112802*ga4 + 6.112802)
ga5 oscil 1.0, 5.928923, isine
ga5 = -0.009110 + (0.009110*ga5 + 0.009110)
ga6 oscil 0.7*ga2, 98.0709 * (1.0 + ga4+ga5), isquare
ga7 oscil 1.0, 0.321814, isine
ga7 = 0.5 + (0.250000*ga7 + 0.250000)
ga8 oscil 1.0, 0.064713, isine
ga8 = 0.2 + (0.650000*ga8 + 0.650000)
ga9 oscil 1.0, 0.290151, isine
ga9 = 0 + (0.500000*ga9 + 0.500000)
ga10 oscil ga8, 117.800316, isine
ga10 = -7.068518 + (7.068518*ga10 + 7.068518)
ga11 oscil 1.0, 4.228622, isine
ga11 = -0.012858 + (0.012858*ga11 + 0.012858)
ga12 oscil 0.7*ga8, 117.6850 * (1.0 + ga10+ga11), ipulse
ga13 oscil 1.0, 0.357507, isine
ga13 = 0.5 + (0.250000*ga13 + 0.250000)
ga14 oscil 1.0, 0.035258, isine
ga14 = 0.2 + (0.650000*ga14 + 0.650000)
ga15 oscil 1.0, 0.193782, isine
ga15 = 0 + (0.500000*ga15 + 0.500000)
ga16 oscil ga14, 146.838181, isine
ga16 = -2.962824 + (2.962824*ga16 + 2.962824)
ga17 oscil 1.0, 5.140841, isine
ga17 = -0.015336 + (0.015336*ga17 + 0.015336)
ga18 oscil 0.7*ga14, 147.1063 * (1.0 + ga16+ga17), ipulse
ga19 oscil 1.0, 0.314994, isine
ga19 = 0.5 + (0.250000*ga19 + 0.250000)
ga20 oscil 1.0, 0.029209, isine
ga20 = 0.2 + (0.650000*ga20 + 0.650000)
ga21 oscil 1.0, 0.165754, isine
ga21 = 0 + (0.500000*ga21 + 0.500000)
ga22 oscil ga20, 86.496086, isine
ga22 = -2.278696 + (2.278696*ga22 + 2.278696)
ga23 oscil 1.0, 3.197605, isine
ga23 = -0.009996 + (0.009996*ga23 + 0.009996)
ga24 oscil 0.7*ga20, 87.0227 * (1.0 + ga22+ga23), isine
ga25 oscil 1.0, 0.491092, isine
ga25 = 0.5 + (0.250000*ga25 + 0.250000)
ga26 oscil 1.0, 0.067755, isine
ga26 = 0.2 + (0.650000*ga26 + 0.650000)
ga27 oscil 1.0, 0.123042, isine
ga27 = 0 + (0.500000*ga27 + 0.500000)
ga28 oscil ga26, 108.671150, isine
ga28 = -3.124433 + (3.124433*ga28 + 3.124433)
ga29 oscil 1.0, 5.443969, isine
ga29 = -0.009270 + (0.009270*ga29 + 0.009270)
ga30 oscil 0.7*ga26, 108.7784 * (1.0 + ga28+ga29), ipulse
ga31 oscil 1.0, 0.357949, isine
ga31 = 0.5 + (0.250000*ga31 + 0.250000)
ga32 oscil 1.0, 0.016048, isine
ga32 = 0.2 + (0.650000*ga32 + 0.650000)
ga33 oscil 1.0, 0.195354, isine
ga33 = 0 + (0.500000*ga33 + 0.500000)
ga34 oscil ga32, 131.591568, isine
ga34 = -5.280500 + (5.280500*ga34 + 5.280500)
ga35 oscil 1.0, 0.754504, isine
ga35 = -0.008855 + (0.008855*ga35 + 0.008855)
ga36 oscil 0.7*ga32, 130.5341 * (1.0 + ga34+ga35), isquare
ga37 oscil 1.0, 0.313709, isine
ga37 = 0.5 + (0.250000*ga37 + 0.250000)
ga38 oscil 1.0, 0.062076, isine
ga38 = 0.2 + (0.650000*ga38 + 0.650000)
ga39 oscil 1.0, 0.150021, isine
ga39 = 0 + (0.500000*ga39 + 0.500000)
ga40 oscil ga38, 245.189392, isine
ga40 = -6.605020 + (6.605020*ga40 + 6.605020)
ga41 oscil 1.0, 0.562804, isine
ga41 = -0.010662 + (0.010662*ga41 + 0.010662)
ga42 oscil 0.7*ga38, 246.9941 * (1.0 + ga40+ga41), isquare
ga43 oscil 1.0, 0.318488, isine
ga43 = 0.5 + (0.250000*ga43 + 0.250000)
ga44 oscil 1.0, 0.049230, isine
ga44 = 0.2 + (0.650000*ga44 + 0.650000)
ga45 oscil 1.0, 0.215335, isine
ga45 = 0 + (0.500000*ga45 + 0.500000)
ga46 oscil ga44, 294.595184, isine
ga46 = -2.620987 + (2.620987*ga46 + 2.620987)
ga47 oscil 1.0, 4.534270, isine
ga47 = -0.016324 + (0.016324*ga47 + 0.016324)
ga48 oscil 0.7*ga44, 296.3929 * (1.0 + ga46+ga47), isquare
ga49 oscil 1.0, 0.370365, isine
ga49 = 0.5 + (0.250000*ga49 + 0.250000)
ga50 oscil 1.0, 0.053639, isine
ga50 = 0.2 + (0.650000*ga50 + 0.650000)
ga51 oscil 1.0, 0.037166, isine
ga51 = 0 + (0.500000*ga51 + 0.500000)
ga52 oscil ga50, 369.547150, isine
ga52 = -4.958570 + (4.958570*ga52 + 4.958570)
ga53 oscil 1.0, 3.247442, isine
ga53 = -0.015216 + (0.015216*ga53 + 0.015216)
ga54 oscil 0.7*ga50, 370.4911 * (1.0 + ga52+ga53), ipulse
ga55 oscil 1.0, 0.369289, isine
ga55 = 0.5 + (0.250000*ga55 + 0.250000)
ga56 oscil 1.0, 0.027421, isine
ga56 = 0.2 + (0.650000*ga56 + 0.650000)
ga57 oscil 1.0, 0.160470, isine
ga57 = 0 + (0.500000*ga57 + 0.500000)
ga58 oscil ga56, 96.500389, isine
ga58 = -7.373360 + (7.373360*ga58 + 7.373360)
ga59 oscil 1.0, 0.781171, isine
ga59 = -0.009184 + (0.009184*ga59 + 0.009184)
ga60 oscil 0.7*ga56, 96.9871 * (1.0 + ga58+ga59), isquare
ga61 oscil 1.0, 0.379046, isine
ga61 = 0.5 + (0.250000*ga61 + 0.250000)
ga62 oscil 1.0, 0.057369, isine
ga62 = 0.2 + (0.650000*ga62 + 0.650000)
ga63 oscil 1.0, 0.295614, isine
ga63 = 0 + (0.500000*ga63 + 0.500000)
ga64 oscil ga62, 117.314400, isine
ga64 = -2.800570 + (2.800570*ga64 + 2.800570)
ga65 oscil 1.0, 3.269137, isine
ga65 = -0.005840 + (0.005840*ga65 + 0.005840)
ga66 oscil 0.7*ga62, 116.3845 * (1.0 + ga64+ga65), isquare
ga67 oscil 1.0, 0.161528, isine
ga67 = 0.5 + (0.250000*ga67 + 0.250000)
ga68 oscil 1.0, 0.045493, isine
ga68 = 0.2 + (0.650000*ga68 + 0.650000)
ga69 oscil 1.0, 0.135014, isine
ga69 = 0 + (0.500000*ga69 + 0.500000)
ga70 oscil ga68, 145.049728, isine
ga70 = -3.716438 + (3.716438*ga70 + 3.716438)
ga71 oscil 1.0, 4.908080, isine
ga71 = -0.006380 + (0.006380*ga71 + 0.006380)
ga72 oscil 0.7*ga68, 145.4807 * (1.0 + ga70+ga71), itriangle
kenv linseg 0,.05,1441.180664,p3-0.1,1441.180664,.05,0
out (ga6)*kenv
kenv linseg 0,.05,429.814911,p3-0.1,429.814911,.05,0
out (ga12)*kenv
kenv linseg 0,.05,600.298035,p3-0.1,600.298035,.05,0
out (ga18)*kenv
kenv linseg 0,.05,1054.168945,p3-0.1,1054.168945,.05,0
out (ga24)*kenv
kenv linseg 0,.05,1885.802612,p3-0.1,1885.802612,.05,0
out (ga30)*kenv
kenv linseg 0,.05,1575.238525,p3-0.1,1575.238525,.05,0
out (ga36)*kenv
kenv linseg 0,.05,1048.019775,p3-0.1,1048.019775,.05,0
out (ga42)*kenv
kenv linseg 0,.05,469.824005,p3-0.1,469.824005,.05,0
out (ga48)*kenv
kenv linseg 0,.05,986.344666,p3-0.1,986.344666,.05,0
out (ga54)*kenv
kenv linseg 0,.05,1637.092773,p3-0.1,1637.092773,.05,0
out (ga60)*kenv
kenv linseg 0,.05,1196.058472,p3-0.1,1196.058472,.05,0
out (ga66)*kenv
kenv linseg 0,.05,1496.355835,p3-0.1,1496.355835,.05,0
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
