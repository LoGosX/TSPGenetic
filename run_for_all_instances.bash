#!/bin/bash

ls datasets/ | grep .txt | xargs -n 1 -P 0 -I {} ./run.bash datasets/{} 10000 250 0.02 0.02 0.005 -50 0
