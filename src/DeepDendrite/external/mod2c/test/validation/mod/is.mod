TITLE Cardiac L-type Calcium channel
: from BEELER & REUTER, J.Physiol, 1977

NEURON {
    THREADSAFE
	SUFFIX Is
	USEION ca READ cai WRITE ica

	USEION cs WRITE ics VALENCE 2
	USEION ns WRITE ins VALENCE 2

	RANGE gsbar, ica, ics
	GLOBAL minf, ninf, mtau, ntau
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
	(mM) = (milli/liter)
	(S) = (siemens)
}

PARAMETER {
	gsbar= 5e-05(S/cm2) <0,1e9> 
}

STATE { : d f
	m n
}

ASSIGNED {
	v (mV)
	celsius (degC) : 37
	cai (mM)
	ica (mA/cm2)
	ics (mA/cm2)
	ins (mA/cm2)
	minf ninf
	mtau (ms)
	ntau (ms)
	lca
}


INITIAL {
	rates(v)
	m = minf
	n = ninf
}

BREAKPOINT {
LOCAL Es
SOLVE states METHOD derivimplicit
	Es = -82.3-13.0287*log(cai)
	ics = gsbar*m*n*(v - Es)
	ica = ics
	ins = -ics
}

DERIVATIVE states {
	rates(v)
	m' = (minf - m)/mtau
	n' = (ninf - n)/ntau
}

UNITSOFF
FUNCTION alp(v(mV),i) (ms) { 
	if (i==0) {
		alp = 0.095*exp(-0.01*(v - 5))/(exp(-0.072*(v - 5))+1)
	}else if (i==1){
		alp = 0.012*exp(-0.008*(v + 28))/(exp(0.15*(v + 28))+1)
	}
}

FUNCTION bet(v(mV),i) { 
	if (i==0) {
		bet = 0.07*exp(-0.017*(v + 44))/(exp(0.05*(v + 44))+1)
	}else if (i==1){
		bet = 0.0065*exp(-0.02*(v + 30))/(exp(-0.2*(v + 30))+1)
	}
}

UNITSON
PROCEDURE rates(v(mV))
{
LOCAL a, b
:TABLE minf, ninf, mtau, ntau DEPEND celsius FROM -100 TO 100 WITH 200
	a = alp(v,0)  b=bet(v,0)
	mtau = 1/(a + b)
	minf = a/(a + b)
	a = alp(v,1)  b=bet(v,1)
	ntau = 1/(a + b)
	ninf = a/(a + b)
}