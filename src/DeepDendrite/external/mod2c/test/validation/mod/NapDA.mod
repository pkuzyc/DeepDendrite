: Persistent Na+ channel under D1
: from Durstewitz & Gabriel (2006), Cerebral Cortex

NEURON {
	SUFFIX NapDA
	USEION na WRITE ina
	RANGE gNapbar, gna, ena
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
}

PARAMETER {
	gNapbar= 0.0022 (mho/cm2) <0,1e9>
	ena = 55 (mV)
}

ASSIGNED {
        v (mV)
	ina (mA/cm2)
	gna (mho/cm2)
}

STATE {
	m 
        h 
}

INITIAL {
	m = malf(v)/(malf(v)+mbet(v))
	h = half(v)/(half(v)+hbet(v))
}

BREAKPOINT {
	SOLVE states METHOD derivimplicit
	gna = gNapbar*m*h
	ina = gna*(v-ena)
}

DERIVATIVE states {
	m' = (1-m)*malf(v)-m*mbet(v)
	h' = ((1-h)*half(v)-h*hbet(v))/2.0
}

UNITSOFF

FUNCTION malf(v(mV)) (/ms) { 
	LOCAL va 
	va=v+12
	if (fabs(va)<1e-04) {
	   malf= -0.2816*(-9.3 - va*0.5)
	} 
	else {
	   malf = -0.2816*va/(-1+exp(-va/9.3))
	}
}

FUNCTION mbet(v(mV)) (/ms) { 
	LOCAL vb 
	vb=v-15
	if (fabs(vb)<1e-04) {
	   mbet = 0.2464*(6 - vb*0.5)
	}
	else {
	   mbet = 0.2464*vb/(-1+exp(vb/6))
	}
}	

FUNCTION half(v(mV)) (/ms) { 
        half= 2.8e-5*(exp(-(v+42.8477)/4.0248))
}

FUNCTION hbet(v(mV)) (/ms) { 
        hbet= 0.02/(1+exp(-(v-413.9284)/148.2589))
}
	
UNITSON
