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
ga1 oscil 1.0, 0.482014, isine
ga1 = -0.3 + (0.300000*ga1 + 0.300000)
ga2 oscil 1.0, 0.277177, isine
ga3 oscil ga1, 766.407934, isine
ga3 = -5.459796 + (5.459796*ga3 + 5.459796)
ga4 oscil 0.7+ga1, 772.6639 * (1.0 + ga3), isawtooth
ga5 oscil 1.0, 0.472957, isine
ga5 = -0.3 + (0.300000*ga5 + 0.300000)
ga6 oscil 1.0, 0.174261, isine
ga7 oscil ga5, 970.067663, isine
ga7 = -2.921657 + (2.921657*ga7 + 2.921657)
ga8 oscil 0.7+ga5, 965.8298 * (1.0 + ga7), isawtooth
ga9 oscil 1.0, 0.352681, isine
ga9 = -0.3 + (0.300000*ga9 + 0.300000)
ga10 oscil 1.0, 0.169778, isine
ga11 oscil ga9, 1149.560374, isine
ga11 = -7.704460 + (7.704460*ga11 + 7.704460)
ga12 oscil 0.7+ga9, 1158.9958 * (1.0 + ga11), isquare
ga13 oscil 1.0, 0.429409, isine
ga13 = -0.3 + (0.300000*ga13 + 0.300000)
ga14 oscil 1.0, 0.291605, isine
ga15 oscil ga13, 164.182435, isine
ga15 = -1.639919 + (1.639919*ga15 + 1.639919)
ga16 oscil 0.7+ga13, 165.5206 * (1.0 + ga15), ipulse
ga17 oscil 1.0, 0.479537, isine
ga17 = -0.3 + (0.300000*ga17 + 0.300000)
ga18 oscil 1.0, 0.093830, isine
ga19 oscil ga17, 205.642547, isine
ga19 = -2.262372 + (2.262372*ga19 + 2.262372)
ga20 oscil 0.7+ga17, 206.9008 * (1.0 + ga19), isawtooth
ga21 oscil 1.0, 0.278029, isine
ga21 = -0.3 + (0.300000*ga21 + 0.300000)
ga22 oscil 1.0, 0.282949, isine
ga23 oscil ga21, 249.687592, isine
ga23 = -1.584071 + (1.584071*ga23 + 1.584071)
ga24 oscil 0.7+ga21, 248.2809 * (1.0 + ga23), isquare
ga25 oscil 1.0, 0.320783, isine
ga25 = -0.3 + (0.300000*ga25 + 0.300000)
ga26 oscil 1.0, 0.193186, isine
ga27 oscil ga25, 599.373226, isine
ga27 = -7.805542 + (7.805542*ga27 + 7.805542)
ga28 oscil 0.7+ga25, 594.2076 * (1.0 + ga27), ipulse
ga29 oscil 1.0, 0.151159, isine
ga29 = -0.3 + (0.300000*ga29 + 0.300000)
ga30 oscil 1.0, 0.055984, isine
ga31 oscil ga29, 707.647210, isine
ga31 = -0.707097 + (0.707097*ga31 + 0.707097)
ga32 oscil 0.7+ga29, 713.0491 * (1.0 + ga31), isine
ga33 oscil 1.0, 0.411857, isine
ga33 = -0.3 + (0.300000*ga33 + 0.300000)
ga34 oscil 1.0, 0.117922, isine
ga35 oscil ga33, 885.377882, isine
ga35 = -7.396440 + (7.396440*ga35 + 7.396440)
ga36 oscil 0.7+ga33, 891.3114 * (1.0 + ga35), isquare
ga37 oscil 1.0, 0.348020, isine
ga37 = -0.3 + (0.300000*ga37 + 0.300000)
ga38 oscil 1.0, 0.209536, isine
ga39 oscil ga37, 177.299233, isine
ga39 = -5.769512 + (5.769512*ga39 + 5.769512)
ga40 oscil 0.7+ga37, 176.0768 * (1.0 + ga39), isawtooth
ga41 oscil 1.0, 0.313993, isine
ga41 = -0.3 + (0.300000*ga41 + 0.300000)
ga42 oscil 1.0, 0.256157, isine
ga43 oscil ga41, 219.375937, isine
ga43 = -5.228164 + (5.228164*ga43 + 5.228164)
ga44 oscil 0.7+ga41, 220.0959 * (1.0 + ga43), isine
ga45 oscil 1.0, 0.176363, isine
ga45 = -0.3 + (0.300000*ga45 + 0.300000)
ga46 oscil 1.0, 0.233341, isine
ga47 oscil ga45, 266.182737, isine
ga47 = -6.682882 + (6.682882*ga47 + 6.682882)
ga48 oscil 0.7+ga45, 264.1151 * (1.0 + ga47), ipulse
kenv linseg 0,.05,673.895691,p3-0.1,673.895691,.05,0
out (ga4)*kenv
kenv linseg 0,.05,1261.942749,p3-0.1,1261.942749,.05,0
out (ga8)*kenv
kenv linseg 0,.05,1955.333130,p3-0.1,1955.333130,.05,0
out (ga12)*kenv
kenv linseg 0,.05,1804.857544,p3-0.1,1804.857544,.05,0
out (ga16)*kenv
kenv linseg 0,.05,1738.602173,p3-0.1,1738.602173,.05,0
out (ga20)*kenv
kenv linseg 0,.05,807.494751,p3-0.1,807.494751,.05,0
out (ga24)*kenv
kenv linseg 0,.05,482.965057,p3-0.1,482.965057,.05,0
out (ga28)*kenv
kenv linseg 0,.05,681.480896,p3-0.1,681.480896,.05,0
out (ga32)*kenv
kenv linseg 0,.05,1210.672363,p3-0.1,1210.672363,.05,0
out (ga36)*kenv
kenv linseg 0,.05,1311.907471,p3-0.1,1311.907471,.05,0
out (ga40)*kenv
kenv linseg 0,.05,1677.917725,p3-0.1,1677.917725,.05,0
out (ga44)*kenv
kenv linseg 0,.05,1617.565430,p3-0.1,1617.565430,.05,0
out (ga48)*kenv
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
