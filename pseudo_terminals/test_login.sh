#!/bin/bash

(echo Start at `date`;
pty login;
echo Stop at `date`) | tee typepty
