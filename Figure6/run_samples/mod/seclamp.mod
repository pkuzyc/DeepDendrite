TITLE SEClamp

NEURON {
    POINT_PROCESS MySEClamp
    ELECTRODE_CURRENT i
    RANGE dur1, amp1, dur2, amp2, dur3, amp3, rs, vc, i
}

UNITS {
    (nA) = (nanoamp)
    (mV) = (millivolt)
    (uS) = (microsiemens)
}

PARAMETER {
    rs = 1 (megohm) <1e-9, 1e9>
    dur1 (ms)
    amp1 (mV)
    dur2 (ms) <0, 1e9>
    amp2 (mV)
    dur3 (ms)
    amp3 (mV)
}

ASSIGNED {
    v (mV)
    i (nA)
    vc (mV)
    tc2 (ms)
    tc3 (ms)
    on
}

INITIAL {
    tc2 = dur1 + dur2
    tc3 = tc2 + dur3
    on = 0
}

BREAKPOINT {
    if (on) {
        i = (vc - v) / rs 
    }
    else {
        i = 0
    }

    on = 1
    if (t < dur1) {
        vc = amp1 + sin(2 * 3.1415926 * 10 * t / 1000)
    }
    else if (t < tc2) {
        vc = amp2
    }
    else if (t < tc3) {
        vc = amp3
    }
    else {
        vc = 0
        on = 0
    }
}








