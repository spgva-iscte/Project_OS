In this part of the project we have a serious of shell scripts: 
  * cria_pacientes.sh (create_pacient.sh)
  * cria_medico.sh (create_doctor.sh)
  * avalia_medicos.sh (evaluate_doctors.sh)
  * stats.sh

Then there is a bash script called menu which will allows to run this different scripts.

# Cria_Paciente.sh
This script shall read the file ect/passwd of the Tigre server. It should identify the pacients that are recorded in this file. The pacients can be identified because they start with the letter 'a' and there's at least one number following after the 'a'. Then the script shall print the first 10 pacients in a file called "pacientes.txt" (pacients.txt). The structure of a pacitent will be
 > ID number;Name;location;phone number;e-mail;balance


# Menu
When we run the menu the user will be presented with a different numerical options and a map to the meaning of each option. While the user does not select the number option 0 the menu will keep on being printed. If the use selects the option 1 the cria_pacientes.sh script will be executed. If the option 2 is selected the user will be asked to give the information to create a new doctor, the name, the number, their speciality and their email. After the cria_medico.sh will be executed in order to create this new doctor with the information it acquired. If the option 3 is selected the user will be asked to give the location and the balance from which they want to  
