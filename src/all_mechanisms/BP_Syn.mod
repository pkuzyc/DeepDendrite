TITLE Senn 2014 Neuron plasticity

NEURON {
	POINT_PROCESS BP_Syn
	RANGE i, g
	ELECTRODE_CURRENT i

	RANGE lr_start, lr_end, layer_flag
    RANGE u_0, u_1, u_2, u_3, u_4, u_5, u_6, u_7, u_8, u_9
    RANGE s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7, s_8, s_9, s_sum, s_u_soma
    RANGE tgt_0, tgt_1, tgt_2, tgt_3, tgt_4, tgt_5, tgt_6, tgt_7, tgt_8, tgt_9
    RANGE g_0, g_1, g_2, g_3, g_4, g_5, g_6, g_7, g_8, g_9
	RANGE u_soma, u_tgt
    RANGE v_gap, PI, delta_w
	RANGE w, learning_rate
}

PARAMETER {
	lr_start (ms)
	lr_end (ms)

    tgt_0 = 0
    tgt_1 = 0
    tgt_2 = 0
    tgt_3 = 0
    tgt_4 = 0
    tgt_5 = 0
    tgt_6 = 0
    tgt_7 = 0
    tgt_8 = 0
    tgt_9 = 0
    u_tgt = 0

    g_0 = 1
    g_1 = 1
    g_2 = 1
    g_3 = 1
    g_4 = 1
    g_5 = 1
    g_6 = 1
    g_7 = 1
    g_8 = 1
    g_9 = 1

    w
	learning_rate = 0.01
	g = 0.01 (uS)
    layer_flag = 0 : 0 hidden layer(i2h)  1 output layer(h2o)  2 compute softmax(h2inter)
}

ASSIGNED {
	has_stdp
	v (mV)
	i (nA)
    
    u_0
    u_1
    u_2
    u_3
    u_4
    u_5
    u_6
    u_7
    u_8
    u_9

    s_sum
    s_u_soma

    s_0
    s_1
    s_2
    s_3
    s_4
    s_5
    s_6
    s_7
    s_8
    s_9

    fa_error

	u_soma
    v_gap
	PI
    delta_w
}

UNITS {
	(nA) = (nanoamp)
	(mV) = (millivolt)
	(uS) = (microsiemens)
}

INITIAL {
	has_stdp = 0
	delta_w = 0
}

BREAKPOINT {
	:at_time(lr_start)
	:at_time(lr_end)
	if (t < lr_start) {
		has_stdp = 0
	}
	else if (t > lr_end) {
		has_stdp = 0
	}
	else {
		has_stdp = 1
	}
    if (layer_flag == 2) {
        s_sum = exp(u_0) + exp(u_1) + exp(u_2) + exp(u_3) + exp(u_4) + exp(u_5) + exp(u_6) + exp(u_7) + exp(u_8) + exp(u_9)
        s_0 = exp(u_0) / s_sum 
        s_1 = exp(u_1) / s_sum 
        s_2 = exp(u_2) / s_sum 
        s_3 = exp(u_3) / s_sum 
        s_4 = exp(u_4) / s_sum 
        s_5 = exp(u_5) / s_sum 
        s_6 = exp(u_6) / s_sum 
        s_7 = exp(u_7) / s_sum 
        s_8 = exp(u_8) / s_sum 
        s_9 = exp(u_9) / s_sum 
        
        i = 0
    }
    else {
        if (has_stdp == 1) {
            fa_error = 0
            if (layer_flag == 1) {
                fa_error = (u_tgt - s_u_soma)
            }
            else {
                if (u_soma > 0) {
                    fa_error = fa_error + (tgt_0 - s_0) * g_0
                    fa_error = fa_error + (tgt_1 - s_1) * g_1
                    fa_error = fa_error + (tgt_2 - s_2) * g_2
                    fa_error = fa_error + (tgt_3 - s_3) * g_3
                    fa_error = fa_error + (tgt_4 - s_4) * g_4
                    fa_error = fa_error + (tgt_5 - s_5) * g_5
                    fa_error = fa_error + (tgt_6 - s_6) * g_6
                    fa_error = fa_error + (tgt_7 - s_7) * g_7
                    fa_error = fa_error + (tgt_8 - s_8) * g_8
                    fa_error = fa_error + (tgt_9 - s_9) * g_9
                }
            }
            if (v_gap > 0) {
                PI = fa_error * v_gap
            }
            else {
                PI = 0
            }
            delta_w = delta_w + learning_rate * PI
        } 
        if (v_gap > 0) {
            i = g * w * v_gap
        }
        else {
            i = 0
        }
    }
}
