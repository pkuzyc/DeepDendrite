TITLE AMPA and NMDA receptor with presynaptic short-term plasticity 


COMMENT
AMPA and NMDA receptor conductance using a dual-exponential profile
presynaptic short-term plasticity based on Fuhrmann et al. 2002
Implemented by Srikanth Ramaswamy, Blue Brain Project, July 2009
GUY: Removed  plasticity and depression

ENDCOMMENT 


NEURON {

        POINT_PROCESS NMDA
        RANGE  tau_r_NMDA, tau_d_NMDA,n_NMDA,gama_NMDA
        RANGE Use
        RANGE i,  i_NMDA,  g_NMDA, e, gmax
        NONSPECIFIC_CURRENT i
}

PARAMETER {

    	n_NMDA = 0.28011 (/mM)	
    	gama_NMDA = 0.062 (/mV) 
	   tau_r_NMDA = 0.3   (ms) : dual-exponential conductance profile
        tau_d_NMDA = 43     (ms) : IMPORTANT: tau_r < tau_d
        Use = 1.0   (1)   : Utilization of synaptic efficacy (just initial values! Use, Dep and Fac are overwritten by BlueBuilder assigned values) 

        e = 0     (mV)  : AMPA and NMDA reversal potential
	    mg = 1   (mM)  : initial concentration of mg2+
    	:gmax = .001 (uS) :1nS weight conversion factor (from nS to uS)
    	u0 = 0 :initial value of u, which is the running value of Use
}

COMMENT
The Verbatim block is needed to generate random nos. from a uniform distribution between 0 and 1 
for comparison with Pr to decide whether to activate the synapse or not
ENDCOMMENT
   

  

ASSIGNED {

        v (mV)
        i (nA)
	i_NMDA (nA)
	g_NMDA (uS)
	factor_NMDA
    mggate
	
}

STATE {

       
	A_NMDA       : NMDA state variable to construct the dual-exponential profile - decays with conductance tau_r_NMDA
    B_NMDA       : NMDA state variable to construct the dual-exponential profile - decays with conductance tau_d_NMDA
}

INITIAL{

    LOCAL  tp_NMDA
	
	A_NMDA = 0
	B_NMDA = 0
        
	tp_NMDA = (tau_r_NMDA*tau_d_NMDA)/(tau_d_NMDA-tau_r_NMDA)*log(tau_d_NMDA/tau_r_NMDA) :time to peak of the conductance
        
	
	
	factor_NMDA = -exp(-tp_NMDA/tau_r_NMDA)+exp(-tp_NMDA/tau_d_NMDA) :NMDA Normalization factor - so that when t = tp_NMDA, gsyn = gpeak
    factor_NMDA = 1/factor_NMDA
   
}

BREAKPOINT {

    SOLVE state METHOD cnexp
	mggate = 1 / (1 + exp(gama_NMDA  * -(v)) * (n_NMDA)) :mggate kinetics - Jahr & Stevens 1990
	g_NMDA = (B_NMDA-A_NMDA) * mggate :compute time varying conductance as the difference of state variables B_NMDA and A_NMDA and mggate kinetics
       
	i_NMDA = g_NMDA*(v-e) :compute the NMDA driving force based on the time varying conductance, membrane potential, and NMDA reversal
	i =  i_NMDA
}

DERIVATIVE state{

    
	A_NMDA' = -A_NMDA/tau_r_NMDA
    B_NMDA' = -B_NMDA/tau_d_NMDA
}





NET_RECEIVE (weight){
     
	A_NMDA = A_NMDA + weight*factor_NMDA
    B_NMDA = B_NMDA + weight*factor_NMDA

              
}




