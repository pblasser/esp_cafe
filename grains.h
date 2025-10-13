

//triggered swoops
//dust
//grains written into a delay with ducking
//grains read out by dust with swoops

//do math with negative offset to minimize pumping
//math by fixed point ints and reshift
//math by db exponential shifting in 32 steps
//or probably better cuz simpler is (x*x>>16)*(sig-2048)>>12
//check c operator precedence on that expression
