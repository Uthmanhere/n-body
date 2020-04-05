# n-body
An n-body simulation for optimization techniques of pipelining, vectorization, muticore, cluster computing and domain specific computing.

# Compilation
```
gcc src/sim_utils.c src/main.c -lm
```
# Options
* t: time interval in between simulation steps.
* N: number of particles.
* I: number of iterations in simulation.
* b: bound size

# Running
```
./a.out -t 0.001 -N 200 -I 10000 -b 1000000
```

# Interface
Open the web application in browser:
```
xdg-open web-app/index.html
```
