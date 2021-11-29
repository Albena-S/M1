invsqrt2pi = 0.398942280401433
sigma = 46.613
mu = 127.464
set xrange [0:255]
set yrange [0:0.015]
plot (invsqrt2pi/sigma)*exp(-0.5*((x-mu)/sigma)**2), "ddp.dat" with lines
