#include <kernel.h>


void omarShellProcess(PROCESS self, PARAM param){
	char ch;
	Keyb_Message msg;
	while(1){
		msg.key_buffer = &ch;
		send(keyb_port, &msg);
		//kprintf("%c",ch);
		poke_w(0xb8000, ch);
	}
	}

void init_shell()
{
	create_process(omarShellProcess,5,0,"Shell");
	resign();
}
