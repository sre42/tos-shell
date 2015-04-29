
#include <kernel.h>


//**************************
//run the train application
//**************************
/**
 * String Concatenation Function
 * @param s1 [Base String 1]
 * @param s2 [String to be added at end]
 */
void str_concat(char* s1, char* s2){
	while (*s1!='\0') s1++;
	while (*s2!='\0') {
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1='\0';

}

void send_ToCom(char* string){
	char command[15];
	COM_Message msg;
	char* temp;

	str_concat(command,string);
	str_concat(command,"\015");
	

	msg.output_buffer =command;
   	msg.len_input_buffer = 0;
   	msg.input_buffer = temp;
   	sleep(10);
   	send(com_port,&msg);

}


/**
 * M#x{CR}
 * #: switch ID
 * X: ‘R’ or ‘G’
 */
void setSwitch(char* switchNum, char* RorG){
	char cmd[5];

	cmd[0] = 'M';
	cmd[1] = *switchNum;
	cmd[2] = *RorG;
	cmd[3] = '\0';
	send_ToCom(cmd);
}


/**
 * Initializing all the switches for Zamboni.
 */
void init_switches(){
	setSwitch('5','G');
	setSwitch('8','G');
	setSwitch('9','R');
	setSwitch('1','G');
	setSwitch('4','G');

}

/**
 * Main Train Process
 */
void train_process(PROCESS self, PARAM param)
{

}

/**
 * Initializing train functions
 * @param wnd [description]
 */
void init_train(WINDOW* wnd)
{
	create_process(train_process, 4, 0, "Train Process");
}