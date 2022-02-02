#!/bin/bash
awk -F ';' '{if($5<5 && $6>6) {print $0}}' ./medicos.txt > lista_negra_medicos.txt
cat lista_negra_medicos.txt
