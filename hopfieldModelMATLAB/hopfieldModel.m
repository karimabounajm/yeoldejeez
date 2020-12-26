%BUILD A HOPFIELD NETWORK WHICH RETRIEVES RANDOM MEMORIES
clear all; %clear all variables
close all; %close all open figure windows

%DEFINE NETWORK PARAMETERS
NumPatt = 1; %Number of memory patterns that network will attempt to store and retrieve
N = 100; %Number of neurons in the network (and number of elements in a memory pattern)
NumTimeSteps = 20; %length of run

%MEMORY PATTERNS
Mem_mat = 2*round(rand(N,NumPatt))-1; %random strings of 1's and -1's

%DEFINE SYNAPTIC WEIGHT MATRIX
W_mat = Mem_mat*Mem_mat';

