
#include <kernel.h>

#define TRAIN_NUMBER "20\0"

static WINDOW train_window=  {0, 0, 80, 10, 0, 0, ' '};
int sleep_ticks = 10;

void clear_train_buffer(void);
void init_switches(void);
void str_concat(char*,char*);
void send_ToCom(char*);
void setSwitch(char*,char*);
void change_direction(void);
void change_speed(char*);
int get_switch_status(char*);
void debug_command(void);
void config1(void);


//************************
//Main train initialization functions are at the bottom of this file.

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


	char clrbuff[5];
	char command[15];


	COM_Message msg;
	char* temp;
	//clear_train_buffer();
	command[0]= '\0';

	clrbuff[0] = 'R';
	clrbuff[1] = '\0';
	str_concat(clrbuff,"\015");
	msg.output_buffer =clrbuff;
   	msg.len_input_buffer = 0;
   	msg.input_buffer = temp;
   		sleep(sleep_ticks);
   	send(com_port,&msg);

	str_concat(command,string);
	str_concat(command,"\015");
	

	msg.output_buffer =command;
   	msg.len_input_buffer = 0;
   	msg.input_buffer = temp;

   	
   	sleep(sleep_ticks);
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
	cmd[1] = switchNum;
	cmd[2] = RorG;
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
/*
Change the direction of the Train
L#D{CR}
#: vehicle ID 

 */
void change_direction(){
	char cmd[5];

	cmd[0] = 'L';
	cmd[1] = '\0';
	str_concat(cmd,TRAIN_NUMBER);
	cmd[3] = 'D';
	cmd[4] = '\0';
	send_ToCom(cmd);
}
/*
Change the speed
of a vehicle. 
L#S#{CR}
1st #: vehicle ID
2nd #: speed, 0-5 
 */
void change_speed(char* s){
	char cmd[6];

	cmd[0] = 'L';
	cmd[1] = '\0';
	str_concat(cmd,TRAIN_NUMBER);
	cmd[3] = 'S';
	cmd[4] = s;
	cmd[5] = '\0';
	send_ToCom(cmd);
}
/*
Clear the s88 memory buffer.
s88 is the device that controls
the contacts. 
R{CR} 
 */
void clear_train_buffer(){
	char cmd[5];

	cmd[0] = 'R';
	cmd[1] = '\0';
	send_ToCom(cmd);
}
/*
Get the status of a contact.
“*1\015” is returned if there
is a vehicle on the contact.
Otherwise, “*0\015” is
returned. 
 */
int get_switch_status(char* c){
	char cmd[10];
	char clrbuff[5];
	clear_train_buffer();
	cmd[0] = 'C';
	cmd[1] = c;
	cmd[2] = '\0';
	str_concat(cmd,"\015");
	
	COM_Message msg;
	char* input;
	int len;
	char* temp;

	//clear
	clrbuff[0] = 'R';
	clrbuff[1] = '\0';
	str_concat(clrbuff,"\015");
	msg.output_buffer =clrbuff;
   	msg.len_input_buffer = 0;
   	msg.input_buffer = temp;
   	sleep(sleep_ticks);
   	send(com_port,&msg);


	msg.output_buffer = cmd;
   	msg.len_input_buffer = len;
   	msg.input_buffer = input;
   	
   	//clear_train_buffer();
   	sleep(100);
   	send(com_port,&msg);
   	if(msg.input_buffer[1]=='1') return 1;
   		else return 0;
}


/**
 * configuration 1 without Zamboni
 */
void config1(){
	change_speed('4');
	setSwitch('4','R');
	setSwitch('3','G');
	//clear_train_buffer();
	while(!get_switch_status('1'));
	change_speed('0');
}




















/** 
 * debugging raw command!
 */
void debug_command(){
	char cmd[6];
	cmd[0] = 'L';
	cmd[1] = '2';
	cmd[2] = '0';
	cmd[3] = 'S';
	cmd[4] = '5';
	cmd[5] = '\0';
	
	send_ToCom(cmd);
}
/**
 * Main Train Process
 */
void train_process(PROCESS self, PARAM param)
{
	wprintf(&train_window,"Starting Train Application\n");
	
	wprintf(&train_window,"Initializing Switches\n");
	//sleep(sleep_ticks);
	clear_train_buffer();
	init_switches();
	config1();

	//debug_command();
	//change_speed('2');
	//setSwitch('5','G');
	//setSwitch('4','G');
	wprintf(&train_window,"Shutting down Train App\n");
	while(1);
}

/**
 * Initializing train functions
 * @param wnd [description]
 */
void init_train(WINDOW* wnd)
{
	create_process(train_process, 4, 0, "Train Process");
}