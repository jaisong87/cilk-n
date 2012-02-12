#!/bin/bash

#$ -V
#$ -cwd
#$ -q development
#$ -pe 12way 12
#$ -N gaussianElemination
#$ -o output-$JOB_NAME
#$ -e error-$JOB_NAME
#$ -M <username>@gmail.com
#$ -m be
#$ -l h_rt=01:00:00

export PATH=$PATH:$HOME/cilk/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/cilk/lib
./gaussianElimination < "SampleIO/ge-in.txt" > output.txt
