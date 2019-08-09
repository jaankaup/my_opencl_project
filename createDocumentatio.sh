#!/bin/bash

rm -r DoxygenOut/*
cd src
doxygen doxy_conf
ssh jaankaup@130.234.208.250 'rm -r /home/jaankaup/public_html/opencl/html'
scp -r ../DoxygenOut/html jaankaup@130.234.208.250:/home/jaankaup/public_html/opencl
cd -
