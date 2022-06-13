TITLE Cardiac second inward current
COMMENT
	modified From DiFrancesco & Noble 1985 Phil Trans R Soc Lond 307:353-398 
    modified for Neuron by FE GANNIER
	francois.gannier@univ-tours.fr (University of TOURS)
ENDCOMMENT
INCLUDE "Unit.inc"
INCLUDE "Volume.inc"
NEURON {
	SUFFIX Isi
	USEION ca READ cao, cai WRITE ica
	USEION k READ ko, ki WRITE ik	
	USEION na READ nao, nai WRITE ina
	
	RANGE P, ica, ik, ina, isi
	GLOBAL minf, mtau, hinf, htau	
}

PARAMETER {
	P = 15	(nA/mM)
	alpha_f2 = 5 (/s)
	Kmf2 = 0.001 (mM)
}

STATE { : d, f, f2
	m n h 
}

ASSIGNED {
	v (mV)
	celsius (degC) : 37
	isi (mA/cm2)
	ica (mA/cm2)
	ik (mA/cm2)
	ina (mA/cm2)
	eca (mV)
	minf ninf hinf
	mtau (ms)
	ntau (ms)
	htau (ms)
	cai (mM)
	cao (mM)
	nai (mM)
	nao (mM)
	ki (mM)
	ko (mM)
	beta_f2 (/s)
}

LOCAL RT
INITIAL {
	RT = (1000)*R*(273.15+celsius)
	rate(v)
	m = minf
	n = ninf
	h = hinf
}

BREAKPOINT { LOCAL com, com2, Eo, icat, inat, ikt
	SOLVE states METHOD derivimplicit
	Eo = v - 50
	com = (1e-06)* P/S*Eo*F/RT * m*n*h
	icat = 4*com /(1 - exp(-2*Eo*F/RT))*(cai*exp(100(mV)*F/RT) - cao*exp(-2*F*Eo/RT))
	ica = icat
	com2 = 0.01*com /(1 - exp(-Eo*F/RT))
	ikt  = com2*(ki*exp(50(mV)*F/RT) - ko*exp(-F*Eo/RT))
	ik = ikt
	inat = com2*(nai*exp(50(mV)*F/RT) - nao*exp(-F*Eo/RT))
	ina = inat
	isi = inat + ikt + icat
}

DERIVATIVE states { : d, f, f2
	rate(v)
	m' = (minf - m)/mtau
	n' = (ninf - n)/ntau
	h' = (0.001)*(alpha_f2 - h * (alpha_f2 + beta_f2))
	beta_f2 = cai * alpha_f2 / Kmf2
}

FUNCTION alp(v(mV), i) (/ms) { : order d, f
LOCAL Eo, E1
	if (i==0) {
		Eo = v + 24
		if (fabs(Eo*1(/mV)) < 1e-5)
		{
			alp = (0.001)*120 (/s)
		}
		else
		{
			alp = (0.001)*30(/s/mV)*Eo / (1 - exp(-Eo/4(mV)))
		}
	} else if (i==1) {
		E1 = v + 34
		if (fabs(E1*1(/mV)) < 1e-5)
		{
			alp = (0.001)*25 (/s)
		}
		else 
		{
			alp = (0.001)*6.25(/mV/s)*E1/(exp(E1/4(mV))-1)
		}
	}
}

FUNCTION bet(v(mV), i) (/ms) { : order d,f
LOCAL Eo
	if (i==0) {
		Eo = v + 24
		if (fabs(Eo*1(/mV)) < 1e-5)
		{
			bet = (0.001)*120 (/s)
		}
		else
		{
			bet = (0.001)*12(/mV/s) *Eo / (exp(Eo/10(mV)) -1)
		}
	} else if (i==1) {
		bet = (0.001)*50(/s)/(1 + exp(-(v + 34)/4(mV)))
	}
}

PROCEDURE rate(v (mV)) { LOCAL a,b :
TABLE minf, mtau, ninf, ntau FROM -100 TO 100 WITH 200
	a = alp(v,0)  b = bet(v,0)
	mtau = 1/(a + b)
	minf = a * mtau
	a = alp(v,1)  b = bet(v,1)
	ntau = 1/(a + b)
	ninf = a * ntau
}
