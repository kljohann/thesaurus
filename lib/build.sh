#!/bin/bash

set -x

cmake -DCMAKE_BUILD_TYPE=Release .. && make
