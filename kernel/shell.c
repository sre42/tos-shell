#include <kernel.h>
static WINDOW shell_window = {0, 11,  80, 14, 0, 0, ' '};
static WINDOW train_window=  {0, 0, 80, 10, 0, 0, ' '};
static WINDOW divider_window=  {0, 10, 80, 11, 0, 0, ' '};
char cmdBuffer[80];
int i=0;
char artType[5];


/**
 * Function that prints Author information upon entering about as a shell command
 */
 void printAbout(){
 	wprintf(&shell_window,"Brought to you by Omar - oshaikh@mail.sfsu.edu ;)\n");
 }

 /**
  * Function that prints all possible commands for the user
  */
 void printHelp(){
 	wprintf(&shell_window,"Welcome to HELP\n");
 	wprintf(&shell_window,"Following are the list of commands that you may use:\n");
 	wprintf(&shell_window,"help                  - for displaying help information\n");
 	wprintf(&shell_window,"ps                    - list all processes\n");
 	wprintf(&shell_window,"cls                   - clear window\n");
 	wprintf(&shell_window,"echo {string}         - Echo the string passed\n");
 	wprintf(&shell_window,"wait {duration}       - Sleep for duration passed\n");
 	wprintf(&shell_window,"train help            -View the train commands\n");
 }
/**
 * Prints Ascii art 
 */
printArt(char* artType){
	if(mystrcmp(artType,"ne")){

	}

}



/**
 * Function that prints the processes information
 */
void printAllProcesses(){
	print_all_processes(&shell_window);
}

/**
 * Function that clears the shell window
 */
void clearShellWindow(){
	clear_window(&shell_window);
}

/**
 * Function that prints the argument passed into the shell command echo
 */
void echoShell(){
	char* toEcho = cmdBuffer+5;
	wprintf(&shell_window,toEcho);
	wprintf(&shell_window,"\n");
}

/**
 * Function that sleeps for the duration passed as an argument
 */
void sleepShell(){
	char timePtr[10];
	ignoreWhiteSpaces(timePtr,5);
	//char* duration = cmdBuffer+5;
	char* duration = timePtr;
	int error=0;
	//To convert it into int.
	int num=0;
	while(*duration != '\0'){
		if (*duration >= '0' && *duration <= '9'){
			num = num * 10 + *duration - '0';
		}else{
			wprintf(&shell_window,"Oops, Not a valid Number! Sleeping for 1 Duration");
			error=1;
			num=1; 
			break;
		}
		duration++;
	}
	if(!error){
		wprintf(&shell_window,"Sleeping for Duration:");
		wprintf(&shell_window,timePtr);	
	} 
	sleep(num);
	wprintf(&shell_window,"\n");
	wprintf(&shell_window,"Just Woke up!\n");
}
/**
 * This function stores the whitespaced removed argument from the command buffer starting at index passed
 * @param s   [pointer to the array inwhich the argument has to be stored]
 * @param num [Index of the argument inside the command buffer]
 */
void ignoreWhiteSpaces(char* s,int num){
	int i=0;
	i+=num;

	while(*(cmdBuffer+i)==' '||*(cmdBuffer+i)=='\t')	i++;
	while(*(cmdBuffer+i)!='\0'){
	 *s=*(cmdBuffer+i);
	 s++;
	 i++;
	}
	*s='\0';
}
/**
 * This function stores the whitespaced removed argument from the command buffer starting at index passed
 * @param s   [pointer to the array inwhich the argument has to be stored]
 * @param num [Index of the argument inside the command buffer]
 * @return [Returns the current counter in the command array]
 */
int ignoreWhiteSpacesReturn(char* s,int num){
	int i=0;
	i+=num;

	while(*(cmdBuffer+i)==' '||*(cmdBuffer+i)=='\t')	i++;
	while(*(cmdBuffer+i)!='\0'){
	 *s=*(cmdBuffer+i);
	 s++;
	 i++;
	}
	*s='\0';
	return i;
}
/**
 * Makes the train GO
 */
void train_go(){

	change_speed('4');

}
/**
 * Makes the train STOP
 */
void train_stop(){

	change_speed('0');

}
/**
 * Func to Start Train Function
 */
void start_train(){
	init_train(&train_window);
}

/**
 * Function to clear the buffer contents
 */
void clrBuffer(){
	int i;
	for ( i= 0; i < 80; ++i)
	{
		cmdBuffer[i] = '\0';
	}

}
/**
 * Basic String compare Function
 * @param1	[User command]
 * @param2	[Predefined TOS command]
 * @return	[Boolean]
 */
BOOL mystrcmp(char* cmd1, char* cmd2){
	while (*cmd1 == *cmd2 && *cmd2 != '\0') {
        cmd1++;
        cmd2++;
    }
    return *cmd2 == '\0';
    /*
    //refactoring code...
    if (*cmd2 == '\0')
    	return 1;
    else
    	return 0;
    	*/
}
/**
 * Execute the command!
 * uses a basic string compare mechanism to find the command.
 */
void executeCmd(){
	char* cmd = &cmdBuffer[0];
	char arg[10];
	char raw[10];
	if(mystrcmp(cmd,"help")){
		printHelp();
	}else if(mystrcmp(cmd,"about")){
		printAbout();
	}else if(mystrcmp(cmd,"cls")){
		clearShellWindow();
	}else if(mystrcmp(cmd,"echo ")){
		echoShell();
	}else if(mystrcmp(cmd,"echo")){
		wprintf(&shell_window,"Invalid command! enter echo {string}\n");
	}else if(mystrcmp(cmd,"wait ")){
		sleepShell();
	}else if(mystrcmp(cmd,"wait")){
		wprintf(&shell_window,"Invalid command! enter wait {duration}\n");
	}else if(mystrcmp(cmd,"ps")){
		printAllProcesses();
	}else if(mystrcmp(cmd,"train ")){
		int count;
		count = ignoreWhiteSpacesReturn(arg,6);
		if(mystrcmp(arg,"auto")){
		start_train();
		}else if(mystrcmp(arg,"stop")){
			if(!isRunning()){
				wprintf(&shell_window,"Train stopping!\n");
				train_stop();
			}else{
				wprintf(&shell_window,"Oops, Train app is running, try again later!\n");
			}
		}else if(mystrcmp(arg,"d")){
			if(!isRunning()){
				wprintf(&shell_window,"Changing Train direction!\n");
				change_direction();
			}else{
				wprintf(&shell_window,"Oops, Train app is running, try again later!\n");
			}
		}else if(mystrcmp(arg,"go")){
			if(!isRunning()){
				wprintf(&shell_window,"Train is on the GO!\n");
				train_go();
			}else{
				wprintf(&shell_window,"Oops, Train app is running, try again later!\n");
			}
		} else if(mystrcmp(arg,"help")){
			wprintf(&shell_window,"Train HELP:::\n");
			wprintf(&shell_window,"train auto               -Starts automatic train Application\n");
			wprintf(&shell_window,"train stop               -Stops the train\n");
			wprintf(&shell_window,"train go                 -Makes the train go\n");
			wprintf(&shell_window,"train d                  -Changes the train direction\n");
			//wprintf(&shell_window,"***BUGGY**train rraw command-Executes the raw command passed as argument\n");
			wprintf(&shell_window,"train help               -displays train help\n");
			wprintf(&shell_window,"help                     -displays Shell help\n");
		}else if(mystrcmp(arg,"rraw")){
			/*
				//RAW COMMAND
				ignoreWhiteSpaces(raw,count);
				int length = 3;
				char input[5];
				train_raw(raw,0,input);
				*/
		}else {
			wprintf(&shell_window,"Entered wrong command, enter \"train help\" for details\n");
		}
	}else if(mystrcmp(cmd,"train")){
		wprintf(&shell_window,"Please enter \"train help\" for details\n");
	}else{
		wprintf(&shell_window,"Invalid command! enter \"help\" or \"train help\" for information\n");
	}

}



/**
 * Entry point for the main Shell Process
 */
 void tosShellProcess(PROCESS self, PARAM param){

 	int shell_window_height=19;

 	

 	char ch;
 	Keyb_Message msg;

 	init_switches();

 	clear_window(kernel_window);

 	wprintf(&shell_window,"WELCOME TO THE TOS SHELL\n");
 	
 	wprintf(&divider_window,"-------------------------------------------------------");
 	
 	wprintf(&train_window,"\n"); 
 	wprintf(&train_window," /$$$$$$$$ /$$$$$$   /$$$$$$\n"); 
	wprintf(&train_window,"|__  $$__//$$__  $$ /$$__  $$\n"); 
	wprintf(&train_window,"   | $$  | $$  \\ $$| $$  \\__/\n"); 
	wprintf(&train_window,"   | $$  | $$  | $$|  $$$$$$ \n"); 
	wprintf(&train_window,"   | $$  | $$  | $$ \\____  $$\n"); 
	wprintf(&train_window,"   | $$  | $$  | $$ /$$  \\ $$\n"); 
	wprintf(&train_window,"   | $$  |  $$$$$$/|  $$$$$$/\n"); 
	wprintf(&train_window,"   |__/   \\______/  \\______/ \n"); 
 	//init_train(&train_window);// For automatic startup of Train App
 	


	/**
	*Start of main loop
	**/
	while(1){
		
		output_char(&shell_window,'#');
		while(1){

			msg.key_buffer = &ch;
			send(keyb_port, &msg);

			switch(ch){
				/*
				checking for backspace
				 */
				case '\b':
                    if (i == 0)
                        continue;
                    i--;
                    wprintf(&shell_window, "%c", ch);
                    break;
                /*
				checking for return
				 */
				case 13:
					cmdBuffer[i]='\0';
					wprintf(&shell_window, "\n");
					executeCmd();
					i=0;
					clrBuffer();

					//wprintf(&shell_window, "\n");
					output_char(&shell_window,'#');
					break;
				/*
				Any other key
				 */
				default:
					cmdBuffer[i] = ch;
					/*
					Checking for overflow...
					 */
					if(i<78){
						i++;
						wprintf(&shell_window, "%c", ch);
					}

					
					break;
					

			}
		}
	}
}


 void init_shell()
 {

 	create_process(tosShellProcess,3,0,"Shell");
 	//init_train(&train_window);
 	resign();
 }
