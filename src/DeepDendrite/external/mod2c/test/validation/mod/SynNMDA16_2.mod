TITLE Voltage-dependent kinetic model of NMDA receptor

COMMENT

This is an especial version of our model in which model would reset itself 
after 590 ms from the start of simulation
some of its rate constants are re-estimated by cure fitting --> CA1 pyramidal cells
Mg-block rate is similar to the NMDA10_2 model because we do not want to model
a Cs++ containing internal solution 

-----------------------------------------------------------------------------

	Kinetic model of NMDA receptors
	===============================

	16-state gating model:
	Clarke and Johnson, 2008
  
              RA2d1                                             RA2d1Mg
                |    - RA2f -                    - RA2fMg -       |
	R -- RA -- RA2 =<        >= RA2O -- RA2OMg =<          >= RA2Mg -- RAMg -- RMg
                |    - RA2s -                    - RA2sMg -       |
              RA2d2                                             RA2d2Mg	
-----------------------------------------------------------------------------

  Based on voltage-clamp recordings of NMDA receptor-mediated currents in 
  nucleated patches of rat layer II/III cortical pyramidal cells 
  in the occipital cortex (Clarke and Johnson, 2008), 
  this model was originally fitted with Matlab to experimental recordings in 
  order to obtain the optimal values for the parameters.

-----------------------------------------------------------------------------

  Release process modeled with an internal alpha function in order to make it compatible 
  with NetCon onbject, and therefore does not require an external release mechanism.
  
-----------------------------------------------------------------------------  
  
  Unit of g is in uS to make the synaptic weights compatible with 
	NEURON's internal methods of modeling synapses (e.x. exp2syn)
  for more information see the chapter 10 of the neuron book
  
  This mod file is written by Keivan Moradi 2012

-----------------------------------------------------------------------------
ENDCOMMENT

INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}

NEURON {
	POINT_PROCESS NMDA16_2
	USEION na READ nao
	RANGE T_max, T, tau, tRel, Erev, synon
	RANGE R,RA,RA2,RA2d1,RA2d2,RA2f,RA2s,O,OMg,RMg,RAMg,RA2Mg,RA2d1Mg,RA2d2Mg,RA2fMg,RA2sMg
	RANGE g, kd1F, kd1B, kd2F, kd2B, csi
	GLOBAL Kcs, kP, kNo, kNi, kMgF, kMgB, ksF,	ksB, kfF, kfB
	NONSPECIFIC_CURRENT i
	THREADSAFE
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

	Erev	= -0.7    	(mV)	: reversal potential
	gmax	= 50  	(pS)	: maximal conductance Johnson & Ascher, 1990
	Mg		= 1  	(mM)	: external magnesium concentration
	
	: alpha function formalism
	tau  = .3 (ms) <1e-9,1e9>
	T_max = 1.5 (mM)		: maximum concentration of neurotransmitter

: Rates
	kon  = 2.83		(/ms /mM)
	koff = 38.1e-3	(/ms)
	: voltage dependent rates
	ksF0 = 48e-3	(/ms)
	ksB0 = 230e-3	(/ms)
	kfF0 = 2836e-3	(/ms)
	kfB0 = 175e-3	(/ms)	: 
	Vdep = 175		(mV)	: sensitivity to membrane voltage
	V0   = -100		(mV)	: the membrane voltage with no voltage-dependetn component
	: kd1F, kd1B, kd2F and kd2B should be found with optimization method since
	: NMDAR in different regions of the brain have different time course
	kd1F = 0.55		(/ms)
	kd1B = 0.081	(/ms)
	kd2F = 0.32319	(/ms)
	kd2B = 0.00020977	(/ms)
: Parameters that control the Mg block as in Antonov99
	Kna  = 34.4		(mM)
	Kcs0 = 0.27		(mM)
	a    = -21		(mV)
	kP0  = 1.10e3	(/ms /mM)
	b    = -55		(mV)
	kNo0 = 1.10e2	(/ms)
	c    = 52.7		(mV)
	kNi0 = 61.8e-3	(/ms)
	d	 = -50		(mV)
	csi  = 148		(mM)	: internal Cs++ concentration (125 mM CsCl + 23 mM CsOH)
}

ASSIGNED {
	v		(mV)	: postsynaptic voltage
	i 		(nA)	: current = g*(v - Erev)
	g 		(uS)	: conductance
	
	T		(mM)	: neurotransmiter concentration in the cleft
	tRel	(ms)	: spiking time of the presynaptic cell
	synon			: turns the synapse on or Off
	w				: weight of synapse
	
	: voltage-dependnent rates
	ksF		(/ms)
	ksB		(/ms)
	kfF		(/ms)
    kfB		(/ms)
	: Mg-block and -unblock rates
	kMgF	(/ms /mM)
	kMgB	(/ms)
	: Parameters that Mg-block and -unblock rates or the so called apparent-block and -unblock
	Kcs		(mM)
	kP		(/ms /mM)
	kNo		(/ms)
	kNi		(/ms)
	nao		(mM)	: Na+ concentration outside the cell, its value should be assigned in the hoc section
}

STATE {
	: Channel states (all fractions)
	R
	RA
	RA2
	RA2d1
	RA2d2
	RA2f
	RA2s
	O
	OMg
	RMg
	RAMg
	RA2Mg
	RA2d1Mg
	RA2d2Mg
	RA2fMg
	RA2sMg
}

INITIAL {
	T = 0
	synon = 0
	tRel = 0
	
	R = 1
	rates(v,t)
	net_send(590, 1)
}

BREAKPOINT {
	SOLVE kstates METHOD sparse

	g = w * gmax * O
	i = g * (v - Erev)
}

KINETIC kstates {
	rates(v,t)
	
    :           RA2d1                                    RA2d1Mg
    :             |    - RA2f -              - RA2fMg -     |
	: R -- RA -- RA2 =<        >= O -- OMg =<          >= RA2Mg -- RAMg -- RMg
    :             |    - RA2s -              - RA2sMg -     |
    :           RA2d2                                    RA2d2Mg	
	~ R 	 <-> RA			((2*kon*T),koff)
	~ RA 	 <-> RA2		((kon*T),(2*koff))
	~ RA2 	 <-> RA2d1		(kd1F,kd1B)
	~ RA2 	 <-> RA2d2		(kd2F,kd2B)
	~ RA2 	 <-> RA2f		(kfF,kfB)
	~ RA2 	 <-> RA2s 		(ksF,ksB)
	~ RA2f 	 <-> O 			(ksF,ksB)
	~ RA2s 	 <-> O			(kfF,kfB)
	~ O 	 <-> OMg 		((kMgF*Mg),kMgB)
	~ OMg 	 <-> RA2fMg		(ksB,ksF)
	~ OMg 	 <-> RA2sMg		(kfB,kfF)
	~ RA2fMg <-> RA2Mg		(kfB,kfF)
	~ RA2sMg <-> RA2Mg		(ksB,ksF)
	~ RA2Mg  <-> RA2d1Mg	(kd1B,kd1F)
	~ RA2Mg  <-> RA2d2Mg	(kd2B,kd2F)
	~ RA2Mg  <-> RAMg		((2*koff),(kon*T))
	~ RAMg	 <-> RMg		(koff,(2*kon*T))

	CONSERVE R+RA+RA2+RA2d1+RA2d2+RA2f+RA2s+O+OMg+RMg+RAMg+RA2Mg+RA2d1Mg+RA2d2Mg+RA2fMg+RA2sMg = 1
}

NET_RECEIVE(weight) {
	if (flag == 0) {
		tRel = t	: resets the alpha function
		synon = 1	: turns the synapse on. 
					: The alpha function does not require to turn off the synase
		w = weight
	}
	if (flag == 1) {
	: this reseting part is temporarily used to fit both recordings at the same time
		R = 1
		RA = 0
		RA2 = 0
		RA2d1 = 0
		RA2d2 = 0
		RA2f = 0
		RA2s = 0
		O = 0
		OMg = 0
		RMg = 0
		RAMg = 0
		RA2Mg = 0
		RA2d1Mg = 0
		RA2d2Mg = 0
		RA2fMg = 0
		RA2sMg = 0
	}
}

PROCEDURE rates(v (mV), t(ms)) {
	T = T_max * (t - tRel) / tau * exp(1 - (t - tRel) / tau) * synon

	: Mg block mechanism similar to Clarke & Johnson, 2008
	: Kcs  = Kcs0 * exp(v/a)
	: kP	 = kP0  * exp(v/b)
	: kNo  = kNo0 * exp(v/c)
	: kNi  = kNi0 * exp(v/d)
	 
	: kMgF = kP / ((1 + nao/Kna) * (1 + nao/Kna + csi/Kcs))
	: kMgB = kNo / (1 + nao/Kna)^2 + kNi
	
	: Mg block mechanism similar to Vargas-Caballero & Robinson, 2004
	kMgF = 610e-3  * exp(1 (/mV) * -v / 17) * 1 (/mM /ms)	: Magnesium Blocking
	kMgB = 5400e-3 * exp(1 (/mV) *  v / 47) * 1 (/ms)		: Magnesium Unblocking

	ksF = ksF0 * exp((v - V0) / Vdep)
	ksB = ksB0 * exp((v - V0) / Vdep * (-1))
	kfF = kfF0 * exp((v - V0) / Vdep)
	kfB = kfB0 * exp((v - V0) / Vdep * (-1))
}
