TITLE detailed model of glutamate NMDA receptors

COMMENT
-----------------------------------------------------------------------------
This is special version of the model that resets itself after 590 ms
from the start of simulation
some of its rate constants are re-estimated by cure fitting --> CA1 pyramidal cells
Binding rate constant is set to be similar to the NMDA16 model

	Kinetic model of NMDA receptors
	===============================

	10-state gating model:
	Vargas-Caballero & Robinson 2004, J Neurosci. 24(27):6171-6180

                   D                 DB
		           |                 |
    C0  -- C1  -- C2  -- O -- OB -- CB2 -- CB1 -- CB0

  Voltage dependence of Mg2+ block and slow voltage dependent unblok:
  Vargas-Caballero & Robinson, 2004, J. Neurosci. 24(27):6171-6180
-----------------------------------------------------------------------------

  Based on voltage-clamp recordings of NMDA receptor-mediated currents in rat
  hippocampal slices (Vargas-Caballero & Robinson 2003 J Neurophysiol 89: 2778-2783), this model 
  was fit directly to experimental recordings in order to obtain the optimal
  values for the parameters (see Vargas-Caballero & Robinson 2004, J. Neurosci. 24(27):6171-6180).

-----------------------------------------------------------------------------

  See details in:

  Destexhe, A., Mainen, Z.F. and Sejnowski, T.J.  Kinetic models of 
  synaptic transmission.  In: Methods in Neuronal Modeling (2nd edition; 
  edited by Koch, C. and Segev, I.), MIT press, Cambridge, 1998, pp 1-25.

  (electronic copy available at http://cns.iaf.cnrs-gif.fr)


  Written by Alain Destexhe and Zach Mainen, 1995
  
-----------------------------------------------------------------------------

  These modifications are made by Keivan Moradi 2007 & 2012:
  - Release process modeled with an internal alpha function in order to make it compatible 
  with NetCon onbject, and therefore does not require an external release mechanism.

  - Unit of g changed from pS to uS to make the synaptic weights compatible with 
	NEURON's internal methods of modeling synapses (e.x. exp2syn)
	
  - gmax is set to 50 Johnson & Ascher, 1990
  
  - Rate constants are corrected to be like the original article
-----------------------------------------------------------------------------
ENDCOMMENT

INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}

NEURON {
	POINT_PROCESS NMDA10_2_2
	RANGE C0, C1, C2, D, O, CB0, CB1, CB2, DB, OB
	RANGE g, gmax, rb, RMgB, RMgU, Rd, Rr, Ro, Rc, Rb, Ru
	RANGE T_max, T, tau, tRel, Erev, synon
	GLOBAL mg
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

	Erev = -0.7	(mV)	: reversal potential
	gmax = 50	(pS)	: maximal conductance
	mg	 = 1	(mM)	: external magnesium concentration
	
: Rates
	Rb	= 2.83		(/mM /ms)	: binding 		
	Ru	= 38.1e-3  	(/ms)		: unbinding		
	Rd	= 4.7161   	(/ms)		: desensitization
	Rr	= 0.16116  	(/ms)		: resensitization 
	Ro	= 0.099631	(/ms)		: opening
	Rc	= 0.056999	(/ms)		: closing	
	
	: alpha function formalism
	tau  = .3 (ms) <1e-9,1e9>
	T_max = 1.5 (mM)			: maximum concentration of neurotransmitter
}

ASSIGNED {
	v		(mV)		: postsynaptic voltage
	i 		(nA)		: current = g*(v - Erev)
	g 		(uS)		: conductance

	rb		(/ms)		: binding
	RMgB	(/ms)
	RMgU	(/ms)
	
	T		(mM)		: neurotransmiter concentration in the cleft
	tRel	(ms)		: spiking time of the presynaptic cell
	synon				: turns the synapse on or Off	(0 = off) (1 = on)
	w					: weight of synapse
}

STATE {
	: Channel states (all fractions)
	C0		: unbound
	C1		: single bound
	C2		: double bound
	D		: desensitized
	O		: open
	CB0		: Blocked unbound
	CB1		: Blocked single bound
	CB2		: Blocked double bound
	DB		: Blocked desensitized
	OB		: Blocked open
}

INITIAL {
	T = 0
	synon = 0
	tRel = 0
	
	rates(v)
	C0	= 1
	C1	= 0
	C2	= 0
	D	= 0
	O	= 0
	CB0	= 0
	CB1	= 0
	CB2	= 0
	DB	= 0
	OB	= 0
	
	net_send(590, 1)
}

BREAKPOINT {
	SOLVE kstates METHOD sparse

	g = w * gmax * O
	i = g * (v - Erev)
}

KINETIC kstates {	
	release(t)
	
	rb = Rb * T 

	rates(v)

	~ C0  <-> C1	((2 * rb),Ru)
	~ C1  <-> C2	(rb,(2 * Ru))
	~ C2  <-> D		(Rd,Rr)
	~ C2  <-> O		(Ro,Rc)
	~ O   <-> OB	(RMgB,RMgU)
	~ OB  <-> CB2	((3*Rc),Ro)
	~ CB2 <-> DB	(Rd,Rr)
	~ CB2 <-> CB1	((2 * Ru),rb)
	~ CB1 <-> CB0	(Ru,(2 * rb))

	CONSERVE C0+C1+C2+D+O+CB0+CB1+CB2+DB+OB = 1
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
		C0	= 1
		C1	= 0
		C2	= 0
		D	= 0
		O	= 0
		CB0	= 0
		CB1	= 0
		CB2	= 0
		DB	= 0
		OB	= 0
	}
}
PROCEDURE release(t(ms)) {
	T = T_max * (t - tRel) / tau * exp(1 - (t - tRel) / tau) * synon
	: VERBATIM
	: return 0;
	: ENDVERBATIM
}

PROCEDURE rates(v(mV)) {
	RMgB = 610e-3 * exp(1 (/mV) * -v / 17) * (mg / 1 (mM)) * 1 (/ms)	: Magnesium Blocking
	RMgU = 5400e-3 * exp(1 (/mV) * v / 47) * 1 (/ms)					: Magnesium Unblocking
}