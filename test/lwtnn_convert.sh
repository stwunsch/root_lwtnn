#!/bin/bash

LWTNN=../lwtnn

python $LWTNN/converters/keras2json.py architecture.json variables.json weights.h5 > lwtnn.json
