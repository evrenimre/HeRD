[![CI](https://github.com/evrenimre/HeRD/actions/workflows/CI.yml/badge.svg)](https://github.com/evrenimre/HeRD/actions/workflows/CI.yml)
![codecov.io](https://codecov.io/gh/evrenimre/HeRD/coverage.svg?branch=main)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)

# HeRD
[API Documentation](https://evrenimre.github.io/HeRD/index.html)

HeRD  is a library for stellar evolution simulators.

## Scope

### In Progress
* Single-star evolution as per:
  * "Zero-Age Main-Sequence Radii and Luminosities as Analytic Functions of Mass and Metallicity," Tout et al., 1996
  * "Comprehensive Analytic Formulae for Stellar Evolution as a Function of Mass and Metallicity," Hurley et al., 2000
  * The original implementation, the SSE package, is available in [AMUSE](https://github.com/amusecode/amuse)

### Planned
* Pre-main sequence evolution
* Planetary systems
* Binary star evolution as per "Evolution of binary stars and the effect of tides on binary populations, " Hurley, 2002.
  * The original implementation, the BSE package, is also available in [AMUSE](https://github.com/amusecode/amuse)

## Building and Installation

### Supported compilers
The code is developed with standard C++ without any compiler-specific extensions. So, in principle, it should compile anywhere. However, my development environment is Linux, and my compiler of choice is g++.

### Building
`CI.yml` action indicates the most up-to-date way to build the library in Ubuntu. I hope to get around to creating a Windows build job, once I figure out how to deal with dependencies.
 
### Dependencies
* Boost
*	Eigen
* Range-v3

Please see `CI.yml` and the action 'install-ubuntu-dependencies' for an up-to-date list.

### Building
From your build directory 

```
cmake -DCMAKE_INSTALL_PREFIX=path-to-install-dir path-to-source
```

### Installation
```
make install
```

## Other

### Motivation
Purely personal: the project is a focusing device for experimenting with new language features and technologies. My background is not in astrophysics. If you have not had enough of experts and want code from real astrophysicists, check out [AMUSE](https://github.com/amusecode/amuse).

### Name
For Hertzsprung-Russell Diagram. The project is a child of the COVID pandemic.

## Development

[Coverage reports](https://app.codecov.io/gh/evrenimre/herd)
