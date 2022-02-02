#!/bin/bash
if [ $# -ne 2 ]; then
	echo "Dados invalidos"
	exit
fi
echo -n "Numero de Pacientes: "
grep ";${1};" pacientes.txt | wc -l
echo -n "Numero de Medicos: "
awk -v saldo=$2 -F ';'  '{if($7 > saldo){print $7}}' ./medicos.txt | wc -l 

