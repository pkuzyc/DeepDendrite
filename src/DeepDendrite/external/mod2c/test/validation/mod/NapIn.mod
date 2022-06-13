TITLE nap

NEURON {
	SUFFIX napIn
	USEION na READ ena WRITE ina
	RANGE  gbar, thegna, htau
	GLOBAL minf, mtau, hinf
}

PARAMETER {
	gbar = .0052085   	(mho/cm2)
    htau = 15	(ms)
	
	eNa = 55 	(mV)		:Golomb et al.
	ena		(mV)            : must be explicitly def. in hoc
	celsius (degC)
	v 		(mV)
}


UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
	(pS) = (picosiemens)
	(um) = (micron)
} 

ASSIGNED {
	ina 		(mA/cm2)
	thegna		(mho/cm2)
	minf 		hinf 		
	mtau (ms)		
}
 

STATE { m h}

: hier eigener Befehl
UNITSOFF

BREAKPOINT {
        SOLVE states METHOD cnexp    		
	trates(v)	
	:Temparaturabhaengigkeit mit modellieren???
	thegna =gbar*m*h :hier geaendert       
	ina = thegna * (v - eNa)
	} 

INITIAL {
	trates(v)
	m=minf  
	h=hinf
}

DERIVATIVE states {   
    trates(v)      
	m' = (minf-m)/mtau
    h' = (hinf-h)/htau

}

PROCEDURE trates(vm) {  
    mtau = 1
	minf = 1/(1+exp(-(v+52.3)/6.8))
    hinf = 1/(1+exp((v+48)/10.0)) : hier aendern
	
}

: hier eigener Befehl

UNITSON
