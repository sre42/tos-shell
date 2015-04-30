
#include <kernel.h>

#define TRAIN_NUMBER "20\0"

static WINDOW train_window=  {0, 0, 80, 10, 0, 0, ' '};
int sleep_ticks = 10;
int config =0;
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


	char command[15];
	command[0] = '\0';
	char temp[10];
	COM_Message msg;
	str_concat(command,string);
	str_concat(command,"\015");
	
	int len;
	msg.output_buffer =command;
   	msg.len_input_buffer = len;
   	msg.input_buffer = temp;

   	
   	sleep(sleep_ticks);
   	send(com_port,&msg);

}
/**
 * A function that takes the raw Train Command as argument and sends it into COM
 * @param cmd [Raw command passed (from shell)]
 * @return 
 */
void train_raw(char* cmd,int length,char* input){
	char command[15];
	command[0] = '\0';
	COM_Message msg;
	str_concat(command,cmd);
	str_concat(command,"\015");
	
	msg.output_buffer =command;
   	msg.len_input_buffer = length;
   	msg.input_buffer = input;

   	
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
/**
 * Get the status of a contact.
	“*1\015” is returned if there
	is a vehicle on the contact.
	Otherwise, “*0\015” is
	returned.
 * @param  c [switch number passed as a string]
 * @return   [1 or 0 based on switch status]
 */
int get_switch_status(char* c){
	char cmd[10];
	clear_train_buffer();
	cmd[0] = 'C';
	cmd[1] = '\0';
	str_concat(cmd,c);
	str_concat(cmd,"\015");
	
	COM_Message msg;
	char* input;
	int len=3;
	

	msg.output_buffer = cmd;
   	msg.len_input_buffer = len;
   	msg.input_buffer = input;
   	
   	sleep(10);
   	send(com_port,&msg);
   	if(msg.input_buffer[1]=='1') return 1;
   		else return 0;
}


/**
 * configuration 1 & 2 without Zamboni
 */
void config1(){
	change_speed('4');
	setSwitch('4','R');
	setSwitch('3','G');
	//clear_train_buffer();
	while(!get_switch_status("1"));
	change_speed('0');
	change_direction();
	change_speed('4');
	setSwitch('5','R');
	setSwitch('6','R');
	while(!get_switch_status("8"));
	change_speed('0');
	wprintf(&train_window,"At home base!!!\n");
}
/**
 * configuration 3 without Zamboni
 */
void config3(){
	change_speed('4');
	setSwitch('5','R');
	setSwitch('6','G');
	setSwitch('7','R');
	while(!get_switch_status("12"));
	change_speed('0');
	change_direction();
	change_speed('4');
	while(!get_switch_status("13"));
	change_speed('0');
	setSwitch('8','G');
	change_direction();
	change_speed('4');
	setSwitch('5','G');
	setSwitch('4','R');
	setSwitch('3','R');
	while(!get_switch_status("5"));
	change_speed('0');
	wprintf(&train_window,"At home base!!!\n");
	}
/**
 * configuration 4 without Zamboni
 */
void config4(){

	change_speed('4');
	setSwitch('5','G');
	setSwitch('8','G');
	setSwitch('9','G');
	while(!get_switch_status("14"));
	change_speed('0');
	change_speed('4');
	while(!get_switch_status("16"));
	sleep(500);
	change_speed('0');

	change_direction();
	change_speed('4');
	setSwitch('4','R');
	setSwitch('3','R');
	while(!get_switch_status("5"));
	change_speed('0');
	wprintf(&train_window,"At home base!!!\n");
}
/**
 * Checks for Zamboni
 * @return [returns the switch at which zamboni was found]
 */
int check_zamboni(){
	int i;
	for(i=0;i<20;i++){
		sleep(10);
		if(get_switch_status("7")){
			return 7;
		}
		sleep(10);
		if(get_switch_status("13")){
			return 13;
		}
	}
	return 0;
}
/**
 * Runs checks to determine the type of config
 * @return [description]
 */
int check_config(){
	if((get_switch_status("8"))&&(get_switch_status("2"))){
		//config1 & config2
		config= 1;
	}else if((get_switch_status("5"))&&(get_switch_status("11"))){
		//config3
		config= 3;
	}else if((get_switch_status("5"))&&(get_switch_status("16"))){
		//config4
		config= 4;
	}

	return config;
}

/**
 * Main Train Process
 */
void train_process(PROCESS self, PARAM param)
{
	wprintf(&train_window,"Starting Train Application\n");
	
	wprintf(&train_window,"Initializing Switches\n");

	init_switches();

	clear_train_buffer();
	//config4();
	int zam=0;
	int config=0;
	zam = check_zamboni();
	if(zam){
		wprintf(&train_window,"Zamboni Found\n");
		if(zam==7){
			wprintf(&train_window,"Zamboni Is rotating Clock-Wise\n");	
		}else if(zam==13){
			wprintf(&train_window,"Zamboni Is rotating Anti Clock-Wise\n");	
		}

	}else wprintf(&train_window,"Zamboni not found\n");
	config=check_config();
	switch(config){
		case 1:
			wprintf(&train_window,"Config is 1 or 2\n");
			if(!zam) config1();
			break;
		case 3:
			wprintf(&train_window,"Config is 3\n");
			if(!zam) config3();
			break;
		case 4:
			wprintf(&train_window,"Config is 4\n");
			if(!zam) config4();
			break;
	}
	

	//config1 with zamboni - working

	if(zam==7&&config==1){
		wprintf(&train_window,"Config 1 With Zamboni\n");
		setSwitch('7','R');
		setSwitch('2','R');
		while(!get_switch_status("10"));
		setSwitch('1','R');
		config1();
	}else if(zam==13&&config==1){
		wprintf(&train_window,"Config 2 With Zamboni\n");
		//config2 with zamboni
		setSwitch('7','R');
		setSwitch('2','R');
		while(!get_switch_status("3"));
		setSwitch('8','R');
		setSwitch('1','R');
		config1();
	}






	wprintf(&train_window,"Shutting down Train App\n");
	while(1);
}

/**
 * Initializing train functions
 * @param wnd [description]
 */
void init_train(WINDOW* wnd)
{
	create_process(train_process, 5, 0, "Train Process");
}