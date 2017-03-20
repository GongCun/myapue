#!/bin/bash

(echo Start at `date`;
pty /bin/bash;
echo Stop at `date`) | tee typepty
