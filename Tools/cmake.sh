#!/bin/sh

cmake -DCMAKE_CXX_COMPILER="/usr/local/bin/iwyu-wrapper" -H../ -B../Build -G "Unix Makefiles"