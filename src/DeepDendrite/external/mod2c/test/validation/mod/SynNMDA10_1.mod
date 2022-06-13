TITLE kinetic NMDA receptor model

COMMENT
-----------------------------------------------------------------------------

	Kinetic model of NMDA receptors 
	best viewed in Notepad++
	===============================

	10-state gating model:
	Kampa et al. (2004) J Physiol
  
	U  --  Cl  --  O
	  \    | \      \
	   \   |  \      \
		UMg -- ClMg - OMg
		   |    |
		   D1   |
		   | \  |
		   |  \ |
		   D2  \|
		    \   D1Mg
			 \  |
			  \ |
			 D2Mg
-----------------------------------------------------------------------------

  Based on voltage-clamp recordings of NMDA receptor-mediated currents in 
  nucleated patches of  rat neocortical layer 5 pyramidal neurons (Kampa 2004), 
  this model was fit with AxoGraph directly to experimental recordings in 
  order to obtain the optimal values for the parameters.

-----------------------------------------------------------------------------

  This mod file does not include mechanisms for the release and time course
  of transmitter; it should to be used in conjunction with a sepearate mechanism
  to describe the release of transmitter and tiemcourse of the concentration
  of transmitter in the synaptic cleft (to be connected to pointer C here).

-----------------------------------------------------------------------------

  See details of NEURON kinetic models in:

  Destexhe, A., Mainen, Z.F. and Sejnowski, T.J.  Kinetic models of 
  synaptic transmission.  In: Methods in Neuronal Modeling (2nd edition; 
  edited by Koch, C. and Segev, I.), MIT press, Cambridge, 1996.


  Written by Bjoern Kampa in 2004 

-----------------------------------------------------------------------------

  These modifications are made by Keivan Moradi 2007 to 2012:
  - Release process modeled with an internal alpha function in order to make it compatible
    with NetCon onbject, and therefore does not require an external release mechanism.

  - Unit of g changed from pS to uS to make the synaptic weights compatible with 
	NEURON's internal methods of modeling synapses (e.x. exp2syn)
	
  - gmax is set to 50 Johnson & Ascher, 1990
  
  - rate constant units changed from (/uM /ms) to (/mM /ms) to remove the annoying coefficients
  
  - valence of Mg is +2. Why it is -2 here?

-----------------------------------------------------------------------------
ENDCOMMENT

INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}

NEURON {
	POINT_PROCESS NMDA10_1
	RANGE T_max, T, tau, tRel, Erev, synon
	RANGE U, Cl, D1, D2, O, UMg, ClMg, D1Mg, D2Mg, OMg
	RANGE g, gmax, rb, rmb, rmu, rbMg,rmc1b,rmc1u,rmc2b,rmc2u
	GLOBAL mg, Rb, Ru, Rd1, Rr1, Rd2, Rr2, Ro, Rc, Rmb, Rmu
	GLOBAL RbMg, RuMg, Rd1Mg, Rr1Mg, Rd2Mg, Rr2Mg, RoMg, RcMg
	GLOBAL Rmd1b,Rmd1u,Rmd2b,Rmd2u,rmd1b,rmd1u,rmd2b,rmd2u
	GLOBAL Rmc1b,Rmc1u,Rmc2b,Rmc2u
	GLOBAL valence, memb_fraction
	NONSPECIFIC_CURRENT i
}

UNITS {
	(nA) = (nanoamp)
	(mV) = (millivolt)
	(pS) = (picosiemens)
	(uS) = (microsiemens)
	(umho) = (micromho)
	(mM) = (milli/liter)
	(uM) = (micro/liter)
}

PARAMETER {

	Erev	= 0    	(mV)	: reversal potential
	gmax	= 50  	(pS)	: maximal conductance
	mg		= 1  	(mM)	: external magnesium concentration
	valence = -2			: parameters of voltage-dependent Mg block
	memb_fraction = 0.8
	
	: alpha function formalism
	tau  = .3 (ms) <1e-9,1e9>
	T_max = 1.5 (mM)		: maximum concentration of neurotransmitter

: Rates

	Rb		= 10	   		(/mM /ms)	: binding 		
	Ru		= 5.6e-3  		(/ms)		: unbinding		
	Ro		= 10e-3   		(/ms)		: opening
	Rc		= 273e-3   		(/ms)		: closing 
	Rd1		= 2.2e-3   		(/ms)		: fast desensitisation
	Rr1		= 1.6e-3   		(/ms)		: fast resensitisation
	Rd2 	= 0.43e-3 		(/ms)		: slow desensitisation
	Rr2 	= 0.5e-3		(/ms)		: slow resensitisation
	Rmb		= 0.05			(/mM /ms)	: Mg binding Open
	Rmu		= 12800e-3		(/ms)		: Mg unbinding Open
	Rmc1b	= 0.00005		(/mM /ms)	: Mg binding Closed
	Rmc1u	= 2.438312e-3	(/ms)		: Mg unbinding Closed
	Rmc2b	= 0.00005		(/mM /ms)	: Mg binding Closed2
	Rmc2u	= 5.041915e-3	(/ms)		: Mg unbinding Closed2
	Rmd1b	= 0.00005		(/mM /ms)	: Mg binding Desens1
	Rmd1u	= 2.98874e-3	(/ms)		: Mg unbinding Desens1
	Rmd2b	= 0.00005		(/mM /ms)	: Mg binding Desens2
	Rmd2u	= 2.953408e-3	(/ms)		: Mg unbinding Desens2
	RbMg	= 10			(/mM /ms)	: binding with Mg
	RuMg	= 17.1e-3		(/ms)		: unbinding with Mg
	RoMg	= 10e-3			(/ms)		: opening with Mg
	RcMg	= 548e-3		(/ms)		: closing with Mg
	Rd1Mg	= 2.1e-3		(/ms)		: fast desensitisation with Mg
	Rr1Mg	= 0.87e-3		(/ms)		: fast resensitisation with Mg
	Rd2Mg	= 0.26e-3		(/ms)		: slow desensitisation with Mg
	Rr2Mg	= 0.42e-3		(/ms)		: slow resensitisation with Mg
}

ASSIGNED {
	v		(mV)	: postsynaptic voltage
	i 		(nA)	: current = g*(v - Erev)
	g 		(uS)	: conductance
	
	T		(mM)	: neurotransmiter concentration in the cleft
	tRel	(ms)	: spiking time of the presynaptic cell
	synon			: turns the synapse on or Off
	w				: weight of synapse

	rb		(/ms)   : binding, [glu] dependent
	rmb		(/ms)	: blocking V and [Mg] dependent
	rmu		(/ms)	: unblocking V and [Mg] dependent
	rbMg	(/ms)	: binding, [glu] dependent
	rmc1b	(/ms)	: blocking V and [Mg] dependent
	rmc1u	(/ms)	: unblocking V and [Mg] dependent
	rmc2b	(/ms)	: blocking V and [Mg] dependent
	rmc2u	(/ms)	: unblocking V and [Mg] dependent
	rmd1b	(/ms)	: blocking V and [Mg] dependent
	rmd1u	(/ms)	: unblocking V and [Mg] dependent
	rmd2b	(/ms)	: blocking V and [Mg] dependent
	rmd2u	(/ms)	: unblocking V and [Mg] dependent
}

STATE {
	: Channel states (all fractions)
	U		: unbound
	Cl		: closed
	D1		: desensitised 1
	D2		: desensitised 2
	O		: open
	UMg		: unbound with Mg
	ClMg	: closed with Mg
	D1Mg	: desensitised 1 with Mg
	D2Mg	: desensitised 2 with Mg
	OMg		: open with Mg
}

INITIAL {
	T = 0
	synon = 0
	tRel = 0
	
	U = 1
}

BREAKPOINT {
	SOLVE kstates METHOD sparse

	g = w * gmax * O
	i = g * (v - Erev)
}

KINETIC kstates {
	release(t)
	
	rb 		= Rb   * T
	rbMg 	= RbMg * T
	
	rmb 	= Rmb 	* mg * exp((1 (/mV) * v-40) * valence * memb_fraction /25)
	rmu 	= Rmu 	* exp((-1 (/mV))*(v-40) * valence * (1-memb_fraction) /25)
	rmc1b 	= Rmc1b * mg * exp((1 (/mV) * v-40) * valence * memb_fraction /25)
	rmc1u 	= Rmc1u * exp((-1 (/mV))*(v-40) * valence * (1-memb_fraction) /25)
	rmc2b 	= Rmc2b * mg * exp((1 (/mV) * v-40) * valence * memb_fraction /25)
	rmc2u 	= Rmc2u * exp((-1 (/mV))*(v-40) * valence * (1-memb_fraction) /25)
	rmd1b 	= Rmd1b * mg * exp((1 (/mV) * v-40) * valence * memb_fraction /25)
	rmd1u 	= Rmd1u * exp((-1 (/mV))*(v-40) * valence * (1-memb_fraction) /25)
	rmd2b 	= Rmd2b * mg * exp((1 (/mV) * v-40) * valence * memb_fraction /25)
	rmd2u 	= Rmd2u * exp((-1 (/mV))*(v-40) * valence * (1-memb_fraction) /25)

	~ U <-> Cl	(rb,Ru)
	~ Cl <-> O	(Ro,Rc)
	~ Cl <-> D1	(Rd1,Rr1)
	~ D1 <-> D2	(Rd2,Rr2)
	~ O <-> OMg	(rmb,rmu)
	~ UMg <-> ClMg 	(rbMg,RuMg)
	~ ClMg <-> OMg 	(RoMg,RcMg)
	~ ClMg <-> D1Mg (Rd1Mg,Rr1Mg)
	~ D1Mg <-> D2Mg (Rd2Mg,Rr2Mg)
	~ U <-> UMg     (rmc1b,rmc1u)
	~ Cl <-> ClMg	(rmc2b,rmc2u)
	~ D1 <-> D1Mg	(rmd1b,rmd1u)
	~ D2 <-> D2Mg	(rmd2b,rmd2u)

	CONSERVE U+Cl+D1+D2+O+UMg+ClMg+D1Mg+D2Mg+OMg = 1
}

NET_RECEIVE(weight) {
	if (flag == 0) {
		tRel = t	: resets the alpha function
		synon = 1	: turns the synapse on. 
					: The alpha function does not require to turn off the synase
		w = weight
	}
}
PROCEDURE release(t(ms)) {
	T = T_max * (t - tRel) / tau * exp(1 - (t - tRel) / tau) * synon
}
