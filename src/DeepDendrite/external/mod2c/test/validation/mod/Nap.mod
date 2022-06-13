TITLE nap


NEURON {
	SUFFIX nap
	USEION na READ ena WRITE ina
	RANGE  gbar, timestauh, timestaum, shifttaum, shifttauh, thegna
	GLOBAL minf, mtau 
	:, hinf, mtau, htau
}

PARAMETER {
	gbar = .0052085   	(mho/cm2)
	
	:q10m=3.1
	:q10h=2.3
	
	timestauh=1
	timestaum=1
	shifttaum=1
	shifttauh=1
	
	
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
	minf 		:hinf 		
	mtau (ms)	:htau (ms) 	
}
 

STATE { m }

: hier eigener Befehl
UNITSOFF

BREAKPOINT {
        SOLVE states METHOD cnexp
	mtau = 1
	minf = (1/(1+exp(-(v+52.3)/6.8))) :midpoint -56.3, slope 7.4	        		
	:trates(v)	
	:Temparaturabhaengigkeit mit modellieren???
	thegna =gbar*m       
	:thegna = gbar*m*m*h
	ina = thegna * (v - eNa)
	} 

INITIAL {
	:trates(v)
	mtau = 1
	minf = (1/(1+exp(-(v+52.3)/6.8))) :midpoint - 52.3 slope 6.8	5.5	        	
	m=minf  
	:h=hinf
}

DERIVATIVE states {   
        :trates(v)      
	mtau = 1
	minf = (1/(1+exp(-(v+52.3)/6.8))) :midpoint - 52.3 (47) slope 6.8	         	
	m' = (minf-m)/mtau
        :h' = (hinf-h)/htau

}

PROCEDURE trates(vm) {  
        :LOCAL  qm,qh,f1,f2,alphah
	
        :qm=q10m^((celsius-24)/10)
	:qh=q10h^((celsius-24)/10)
	
	:f1=1/(1+exp(-(v-7.63)/28.47))+0.01
	:f2=62.82/(1+exp((v+37.02)/5.27))+3.78

	:mtau=f1*f2/qm*timestaum+shifttaum
	:minf = (1/(1+exp(-(v+31.4)/8.8)))^0.5
	:hinf = 1/(1+exp((v+72)/3.7))
	
	:alphah=0.0021/(1+exp((v+65.77)/4.32))
	:mtau = 1
	:minf = (1/(1+exp(-(v+41)/3)))	        
	
	:htau = hinf/(qh*alphah)*timestauh+shifttauh
}

: hier eigener Befehl

UNITSON
