#include <kernel.h>
static WINDOW shell_window = {0, 10,  80, 25, 0, 0, ' '};
char currentCommandBuffer[80];//max line length


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
 	wprintf(&shell_window,"help  				 - for displaying help information\n");
 	wprintf(&shell_window,"ps     				 - list all processes\n");
 	wprintf(&shell_window,"cls    				 - clear window\n");
 	wprintf(&shell_window,"echo (argument)       - clear window\n");
 	wprintf(&shell_window,"wait (argument)       - clear window\n");
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

}

/**
 * Function that sleeps for the duration passed as an argument
 */
void sleepShell(){

}


/**
 * FUnction to clear the buffer contents
 */
void clrBuffer(){
	int i;
	for ( i= 0; i < 80; ++i)
	{
		currentCommandBuffer[i] = 0;
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

	/**
	*Start of main loop
	**/
	while(1){
		
		output_char(&shell_window,'#');
		

		shell_window_height++;
		while(1){

			msg.key_buffer = &ch;
			send(keyb_port, &msg);
			switch(ch){
				case 13:
					wprintf(&shell_window, "\n");
					output_char(&shell_window,'#');
					break;
				default:
					wprintf(&shell_window, "%c", ch);
					break;

			}
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

 	resign();
 }
