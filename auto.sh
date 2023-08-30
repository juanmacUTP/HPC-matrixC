#!/usr/bin/env bash

for t in {1..10};
do
  for matrixSize in 10 100 200 400 600 800 1000 2000;
      do
./matrix $matrixSize >> matrixOutputNotVerboseV4.log
      done
done
