#!/bin/bash
if [ $# -ne 4 ]; then
	echo "Dados não válidos"
	exit
fi
nome=$1
numero=$2
especialidade=$3
email=$4
if [ -f medicos.txt ]; then
	if ! grep ";$numero;" medicos.txt; then
		echo "$nome;$numero;$especialidade;$email;0;0;0" >> medicos.txt
	else 
		echo "O médico com esse número de cédula já existe"
	fi
else
	echo "$nome;$numero;$especialidade;$email;0;0;0" > medicos.txt
fi
cat ./medicos.txt 
