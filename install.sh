#!/bin/sh
rm config.h && make clean && make && PREFIX=~/.local make install
