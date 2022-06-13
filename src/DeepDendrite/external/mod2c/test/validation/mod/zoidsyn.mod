COMMENT
  zoidsyn.mod
  Generates a train of symmetrically trapeZoidal conductance changes.
  User specifies trf (duration of rise/fall ramps), tp (duration of plateau),
  start (onset of first trapezoid), interval, i.e. period between 
  trapezoid onsets), and number of trapezoids.
  Error checking in INIT block ensures that interval is longer 
  than trapezoid duration.
  4/1/2012 NTC, minor modifications 4/3/2012 TMM
ENDCOMMENT

NEURON {
  POINT_PROCESS ZoidSyn
  RANGE trf, tp
  RANGE start, interval, number
  RANGE e, gmax, g, i
  NONSPECIFIC_CURRENT i
}

UNITS {
  (mV) = (millivolt)
  (nS) = (nanosiemens)
  (nA) = (nanoamp)
}

PARAMETER {
  trf (ms) <0, 1e9> : duration of rising and falling phases
  tp  (ms) <0, 1e9> : duration of plateau
  start (ms) <0, 1e9> : latency of first transient
  interval (ms) <0, 1e9> : period, i.e. interval between transient onsets
  number : how many to deliver
  e   (mV) : reversal potential
  gmax (nS) <0, 1e9> : conductance during plateau
}

ASSIGNED {
  v (mV)
  i (nA)
  on
  tally : how many more to deliver
  m (1/ms)
  b (1)
  dur (ms) : sum of twice the rise/fall time trp plus tp
  t0 (ms)
  g (nS)
}

INITIAL {
  if (trf <= 0) {
    trf = 1
UNITSOFF
    printf("trf must be longer than 0\n")
    printf("trf has been increased to %g ms\n", trf)
UNITSON
  }
  if (tp < 0) {
    tp = 0
UNITSOFF
    printf("tp must not be negative\n")
    printf("tp has been changed to %g ms\n", tp)
UNITSON
  }
  dur = 2*trf + tp
  if (interval <= dur) {
    interval = dur + 1 (ms)
UNITSOFF
    printf("interval must be longer than trapezoid duration\n")
    printf("interval has been increased to %g ms\n", interval)
UNITSON
  }
  on = 0
  m = 0
  b = 0
  tally = number
  if (tally > 0) {
    net_send(start, 1)
    tally = tally - 1
  }
}

BREAKPOINT {
  g = gmax * (m*(t-t0) + b)
  i = (0.001)*g*(v-e)
}

NET_RECEIVE (w) {
  if ((on == 0) && (flag == 1)) {
    : enter rising phase
    t0 = t
    m = 1/trf
    b = 0
    on = 1
    : prepare for plateau phase
    net_send(trf, 2)
  }
  if (flag == 2) {
    : enter plateau
    m = 0
    b = 1
    : prepare for falling phase
    net_send(tp, 3)
  }
  if (flag == 3) {
    : enter falling phase
    t0 = t
    m = -1/trf
    b = 1
    : prepare to end
    net_send(trf, 4)
  }
  if (flag == 4) {
    : end
    m = 0
    b = 0
    on = 0
    if (tally > 0) {
      : prepare to turn it on again
      net_send(interval - dur, 1)
      tally = tally - 1
    }
  }
}
