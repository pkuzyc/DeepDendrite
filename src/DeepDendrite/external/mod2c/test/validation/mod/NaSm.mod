TITLE A slow Sodium current
 
COMMENT
  Used in Role of a Striatal Slowly Inactivating Potassion Current in Short-term 
  Facilitation of Corticostriatal Inputs" A computer Simulation Study" (Mahon et al. 2000)
  Implemented by Kevin M. Biddell kevin.biddell@gmail.com
  7/17/06
NOTE: 1S=1mho Neuron wants the units in mhos not millisiemens, please note the conversion!
ENDCOMMENT
 
UNITS {
        (mA) = (milliamp)
        (mV) = (millivolt)
}
 
NEURON {
 	SUFFIX NaSm
	USEION na WRITE ina
	RANGE gnasmbar, gnasm, minf, mtau
}
 
INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}
 
PARAMETER {
  	
	ena	= 40	(mV)
	gnasmbar= 0.00011 (mho/cm2) :0.11mS
	Etemp	= 21	:delord correspondence 11/15/06
	Vsm	= -16.0
	ksm	= 9.4
	tom	= 637.8
	Vtm	= -33.5
	ktm	= 26.3
	
		     
}
 
STATE {
        m
}
 
ASSIGNED {
	v  (mV)
        ina (mA/cm2)
	celsius		(degC)
 	minf
	mtau
	gnasm
}
 
BREAKPOINT {
        SOLVE states METHOD cnexp
        gnasm = gnasmbar*m
        ina = gnasm*(v - ena)
  
}
 
UNITSOFF
 
INITIAL {
	rates(v)
	m= minf
	
}

DERIVATIVE states {  :Computes states variable m at the current v and dt.
        rates(v)      
       
	m' = ( minf - m ) / mtau
}
 
PROCEDURE rates(v) {  :Computes rate and other constants at current v. Call once from HOC to initialize inf at resting v.
        LOCAL  q10,tadj
        q10 = 2.5
	tadj=q10^((celsius-Etemp)/10)
        minf=1/(1+exp(-(v-Vsm)/ksm))
	mtau=tom/(exp(-(v-Vtm)/ktm)+exp((v-Vtm)/ktm))/tadj
      
}
 
UNITSON

