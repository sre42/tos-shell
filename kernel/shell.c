#include <kernel.h>
static WINDOW shell_window = {0, 10,  80, 15, 0, 0, ' '};
static WINDOW train_window=  {0, 0, 80, 10, 0, 0, ' '};
char cmdBuffer[80];//currentCommandBuffer[max line length]
int i=0;


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
 	wprintf(&shell_window,"echo {string}       - Echo the string passed\n");
 	wprintf(&shell_window,"wait {duration}       - Sleep for duration passed\n");
 	wprintf(&shell_window,"train 					-Start the train App\n");
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
 * Function that slseeps for the duration passed as an argument
 */
void sleepShell(){
	char* duration = cmdBuffer+5;
	//To convert it into int.
	int num=0;
	while(*duration != '\0'){
		if (*duration >= '0' && *duration <= '9'){
			num = num * 10 + *duration - '0';
		}else break;
		duration++;
	}
	wprintf(&shell_window,"Sleeping for Duration:");
	wprintf(&shell_window,cmdBuffer+5);
	sleep(num);
	wprintf(&shell_window,"\n");
	wprintf(&shell_window,"Just Woke up!\n");
}

/**
 * Func to Start Train Function
 */
void start_train(){
	//init_train(&train_window);
}

/**
 * FUnction to clear the buffer contents
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
 * @param1	User command 
 * @param2	Predefined TOS command
 * @return	Boolean
 */
BOOL mystrcmp(char* cmd1, char* cmd2){
	while (*cmd1 == *cmd2 && *cmd2 != '\0') {
        cmd1++;
        cmd2++;
    }
    return *cmd2 == '\0';
    /*
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
	}else if(mystrcmp(cmd,"train")){
		start_train();
	}else{
		wprintf(&shell_window,"Invalid command! enter help for information\n");
	}

}



/**
 * Entry point for the main Shell Process
 */
 void tosShellProcess(PROCESS self, PARAM param){

 	int shell_window_height=19;



 	char ch;
 	Keyb_Message msg;



 	clear_window(kernel_window);
 	wprintf(&shell_window,"WELCOME TO THE TOS SHELL\n");
 	init_train(&train_window);
	/**
	*Start of main loop
	**/
	while(1){
		
		output_char(&shell_window,'#');
		

		//shell_window_height++;
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
				case 13:
					cmdBuffer[i]='\0';
					wprintf(&shell_window, "\n");
					executeCmd();
					i=0;
					clrBuffer();

					//wprintf(&shell_window, "\n");
					output_char(&shell_window,'#');
					break;
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
			//executeCmd();
			/*if(ch == '\r'){
				shell_window.cursor_x = 0;
				shell_window.cursor_y++;
				if(shell_window_height==19){
					scroll_window(&shell_window);
					shell_window_height--;
				}
				break;
				
			}else {

				output_char(&shell_window,ch);
			}*/
		}
	}
}


 void init_shell()
 {

 	create_process(tosShellProcess,5,0,"Shell");
 	//init_train(&train_window);
 	resign();
 }
