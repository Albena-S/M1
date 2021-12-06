invsqrt2pi = 0.398942280401433
sigma = 25.7244
b=sigma/sqrt(2)
mu = 128.441
set xrange [0:255]
set yrange [0:0.015]
plot (1/(2*b))*exp(-(abs(mu-x)/b)), "ddp.dat" with lines
