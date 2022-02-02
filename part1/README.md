# First Part
In this part of the project we have a serious of shell scripts: 
  * cria_pacientes.sh (create_pacient.sh)
  * cria_medico.sh (create_doctor.sh)
  * avalia_medicos.sh (evaluate_doctors.sh)
  * stats.sh

Then there is a bash script called menu which will allows to run this different scripts.

## Cria_Paciente.sh
This script shall read the file ect/passwd of the Tigre server. It should identify the pacients that are recorded in this file. The pacients can be identified because they start with the letter 'a' and there's at least one number following after the 'a'. Then the script shall print the first 10 pacients in a file called "pacientes.txt" (pacients.txt). The structure of a pacitent will be
 > ID Number; Name; Location; Phone Number; Email; Balance
The ID number will be the numbers that appear after the 'a'. The name will be the 5th argument after a "," or a ":". Name and location are not yet know. The email shall be generated with "a + ID number"@iscte-iul.pt. The initial balance of each pacient is 100.

## Cria_Medico.sh
This script will receive 4 arguments, name, ID number, speciality, email. If it does not receive the 4 arguments it will print a message saying the data is invalid. Then it will check if there is a filed called "medicos.txt". If there is it will check if there is already a doctor with that ID number. If the doctor already exists it will let the user know that the ID number is already taken. If there isn't it will add the new doctor to the list of doctors. If the file did not existing previously it will create the file and add the first doctor. The form of the doctor is the following:
> Name; ID number; Speciality; Email; Rating; Number Appointments Given; Balance
The rating, number of appointments and balance all start as 0.

## Avalia_Medico.sh
This script will check if there is any doctor who has given more than 6 appointments and its rating is lower than 5. If so it will update the file "lista_negra_medicos.txt" (doctors_blacklist.txt) with this doctors.

## Stats.sh
This script requires 2 arguments, one location and one balance. If the script does not receive the 2 arguments it will print the message invalid data. The script returns the number of pacients from the location indicated and the number of doctors which have the balanced given.

## Menu
When we run the menu the user will be presented with a different numerical options and a map to the meaning of each option. While the user does not select the number option 0 the menu will keep on being printed. If the use selects the option 1 the cria_pacientes.sh script will be executed. If the option 2 is selected the user will be asked to give the information to create a new doctor, the name, the number, their speciality and their email. After the cria_medico.sh will be executed in order to create this new doctor with the information it acquired. If the option 3 is selected the user will be asked to give a location and a balance from and then the stats.sh script will be executed. If the user selects the option 4 the doctors blacklist wil be updated through running the script avalia_medico.sh. 
