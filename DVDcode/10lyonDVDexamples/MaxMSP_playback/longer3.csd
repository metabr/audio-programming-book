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
ga1 oscil 1.0, 0.429164, isine
ga1 = 0.5 + (0.250000*ga1 + 0.250000)
ga2 oscil 1.0, 0.042758, isine
ga2 = 0.2 + (1.900000*ga2 + 1.900000)
ga3 oscil 1.0, 0.143574, isine
ga3 = 0 + (0.500000*ga3 + 0.500000)
ga4 oscil ga2, 174.778687, isine
ga4 = -2.792120 + (2.792120*ga4 + 2.792120)
ga5 oscil 1.0, 1.285775, isine
ga5 = -0.018584 + (0.018584*ga5 + 0.018584)
ga6 oscil 0.7*ga2, 175.4687 * (1.0 + ga4+ga5), isine
ga7 oscil 1.0, 0.248714, isine
ga7 = 0.5 + (0.250000*ga7 + 0.250000)
ga8 oscil 1.0, 0.081960, isine
ga8 = 0.2 + (1.900000*ga8 + 1.900000)
ga9 oscil 1.0, 0.258180, isine
ga9 = 0 + (0.500000*ga9 + 0.500000)
ga10 oscil ga8, 209.221268, isine
ga10 = -3.185024 + (3.185024*ga10 + 3.185024)
ga11 oscil 1.0, 5.076172, isine
ga11 = -0.009867 + (0.009867*ga11 + 0.009867)
ga12 oscil 0.7*ga8, 210.5624 * (1.0 + ga10+ga11), isine
ga13 oscil 1.0, 0.398747, isine
ga13 = 0.5 + (0.250000*ga13 + 0.250000)
ga14 oscil 1.0, 0.169286, isine
ga14 = 0.2 + (1.900000*ga14 + 1.900000)
ga15 oscil 1.0, 0.241959, isine
ga15 = 0 + (0.500000*ga15 + 0.500000)
ga16 oscil ga14, 264.860657, isine
ga16 = -0.689572 + (0.689572*ga16 + 0.689572)
ga17 oscil 1.0, 2.969426, isine
ga17 = -0.005496 + (0.005496*ga17 + 0.005496)
ga18 oscil 0.7*ga14, 263.2031 * (1.0 + ga16+ga17), isine
ga19 oscil 1.0, 0.187952, isine
ga19 = 0.5 + (0.250000*ga19 + 0.250000)
ga20 oscil 1.0, 0.031468, isine
ga20 = 0.2 + (1.900000*ga20 + 1.900000)
ga21 oscil 1.0, 0.018656, isine
ga21 = 0 + (0.500000*ga21 + 0.500000)
ga22 oscil ga20, 199.147736, isine
ga22 = -5.139913 + (5.139913*ga22 + 5.139913)
ga23 oscil 1.0, 4.495571, isine
ga23 = -0.017428 + (0.017428*ga23 + 0.017428)
ga24 oscil 0.7*ga20, 197.7583 * (1.0 + ga22+ga23), isine
ga25 oscil 1.0, 0.449632, isine
ga25 = 0.5 + (0.250000*ga25 + 0.250000)
ga26 oscil 1.0, 0.102289, isine
ga26 = 0.2 + (1.900000*ga26 + 1.900000)
ga27 oscil 1.0, 0.276559, isine
ga27 = 0 + (0.500000*ga27 + 0.500000)
ga28 oscil ga26, 248.816818, isine
ga28 = -2.577812 + (2.577812*ga28 + 2.577812)
ga29 oscil 1.0, 4.557027, isine
ga29 = -0.008282 + (0.008282*ga29 + 0.008282)
ga30 oscil 0.7*ga26, 247.1979 * (1.0 + ga28+ga29), isine
ga31 oscil 1.0, 0.414237, isine
ga31 = 0.5 + (0.250000*ga31 + 0.250000)
ga32 oscil 1.0, 0.114890, isine
ga32 = 0.2 + (1.900000*ga32 + 1.900000)
ga33 oscil 1.0, 0.033062, isine
ga33 = 0 + (0.500000*ga33 + 0.500000)
ga34 oscil ga32, 294.864807, isine
ga34 = -3.566298 + (3.566298*ga34 + 3.566298)
ga35 oscil 1.0, 1.955477, isine
ga35 = -0.014454 + (0.014454*ga35 + 0.014454)
ga36 oscil 0.7*ga32, 296.6375 * (1.0 + ga34+ga35), isine
ga37 oscil 1.0, 0.423488, isine
ga37 = 0.5 + (0.250000*ga37 + 0.250000)
ga38 oscil 1.0, 0.159654, isine
ga38 = 0.2 + (1.900000*ga38 + 1.900000)
ga39 oscil 1.0, 0.234614, isine
ga39 = 0 + (0.500000*ga39 + 0.500000)
ga40 oscil ga38, 174.586975, isine
ga40 = -6.376842 + (6.376842*ga40 + 6.376842)
ga41 oscil 1.0, 5.577785, isine
ga41 = -0.009750 + (0.009750*ga41 + 0.009750)
ga42 oscil 0.7*ga38, 173.1862 * (1.0 + ga40+ga41), isine
ga43 oscil 1.0, 0.463962, isine
ga43 = 0.5 + (0.250000*ga43 + 0.250000)
ga44 oscil 1.0, 0.013387, isine
ga44 = 0.2 + (1.900000*ga44 + 1.900000)
ga45 oscil 1.0, 0.160542, isine
ga45 = 0 + (0.500000*ga45 + 0.500000)
ga46 oscil ga44, 218.140884, isine
ga46 = -5.463530 + (5.463530*ga46 + 5.463530)
ga47 oscil 1.0, 4.304079, isine
ga47 = -0.016162 + (0.016162*ga47 + 0.016162)
ga48 oscil 0.7*ga44, 216.4828 * (1.0 + ga46+ga47), isine
ga49 oscil 1.0, 0.344511, isine
ga49 = 0.5 + (0.250000*ga49 + 0.250000)
ga50 oscil 1.0, 0.026238, isine
ga50 = 0.2 + (1.900000*ga50 + 1.900000)
ga51 oscil 1.0, 0.264964, isine
ga51 = 0 + (0.500000*ga51 + 0.500000)
ga52 oscil ga50, 259.130920, isine
ga52 = -6.191062 + (6.191062*ga52 + 6.191062)
ga53 oscil 1.0, 4.228874, isine
ga53 = -0.015390 + (0.015390*ga53 + 0.015390)
ga54 oscil 0.7*ga50, 259.7793 * (1.0 + ga52+ga53), isine
ga55 oscil 1.0, 0.232555, isine
ga55 = 0.5 + (0.250000*ga55 + 0.250000)
ga56 oscil 1.0, 0.042923, isine
ga56 = 0.2 + (1.900000*ga56 + 1.900000)
ga57 oscil 1.0, 0.256662, isine
ga57 = 0 + (0.500000*ga57 + 0.500000)
ga58 oscil ga56, 355.895874, isine
ga58 = -6.811990 + (6.811990*ga58 + 6.811990)
ga59 oscil 1.0, 5.703827, isine
ga59 = -0.011333 + (0.011333*ga59 + 0.011333)
ga60 oscil 0.7*ga56, 356.2832 * (1.0 + ga58+ga59), isine
ga61 oscil 1.0, 0.234667, isine
ga61 = 0.5 + (0.250000*ga61 + 0.250000)
ga62 oscil 1.0, 0.033951, isine
ga62 = 0.2 + (1.900000*ga62 + 1.900000)
ga63 oscil 1.0, 0.238085, isine
ga63 = 0 + (0.500000*ga63 + 0.500000)
ga64 oscil ga62, 446.529327, isine
ga64 = -1.467524 + (1.467524*ga64 + 1.467524)
ga65 oscil 1.0, 5.469440, isine
ga65 = -0.012318 + (0.012318*ga65 + 0.012318)
ga66 oscil 0.7*ga62, 445.3540 * (1.0 + ga64+ga65), isine
ga67 oscil 1.0, 0.429780, isine
ga67 = 0.5 + (0.250000*ga67 + 0.250000)
ga68 oscil 1.0, 0.132561, isine
ga68 = 0.2 + (1.900000*ga68 + 1.900000)
ga69 oscil 1.0, 0.223340, isine
ga69 = 0 + (0.500000*ga69 + 0.500000)
ga70 oscil ga68, 536.386536, isine
ga70 = -6.810079 + (6.810079*ga70 + 6.810079)
ga71 oscil 1.0, 2.482497, isine
ga71 = -0.005108 + (0.005108*ga71 + 0.005108)
ga72 oscil 0.7*ga68, 534.4248 * (1.0 + ga70+ga71), isine
kenv linseg 0,.05,445.812897,p3-0.1,445.812897,.05,0
out (ga6)*kenv
kenv linseg 0,.05,1286.008179,p3-0.1,1286.008179,.05,0
out (ga12)*kenv
kenv linseg 0,.05,1242.324097,p3-0.1,1242.324097,.05,0
out (ga18)*kenv
kenv linseg 0,.05,422.507324,p3-0.1,422.507324,.05,0
out (ga24)*kenv
kenv linseg 0,.05,688.866760,p3-0.1,688.866760,.05,0
out (ga30)*kenv
kenv linseg 0,.05,1660.867554,p3-0.1,1660.867554,.05,0
out (ga36)*kenv
kenv linseg 0,.05,1375.923096,p3-0.1,1375.923096,.05,0
out (ga42)*kenv
kenv linseg 0,.05,585.341553,p3-0.1,585.341553,.05,0
out (ga48)*kenv
kenv linseg 0,.05,697.653381,p3-0.1,697.653381,.05,0
out (ga54)*kenv
kenv linseg 0,.05,1284.084351,p3-0.1,1284.084351,.05,0
out (ga60)*kenv
kenv linseg 0,.05,1977.649536,p3-0.1,1977.649536,.05,0
out (ga66)*kenv
kenv linseg 0,.05,1187.230957,p3-0.1,1187.230957,.05,0
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
