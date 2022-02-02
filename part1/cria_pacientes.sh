#!/bin/bash
awk -F '[:,]' '/^a[0-9]/ {split($1,n,"a"); print n[2] ";" $5 ";" ";" ";" $1 "@iscte-iul.pt" ";" "100"}' /etc/passwd | head -10 > pacientes.txt
