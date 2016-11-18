
struct Cmd {
	char op ;
	int arg ;
} ;


static struct Cmd child_cmds[] = {
	{ .op = 'z', .arg = 3 },
	{ .op = 'e', .arg = 0 },
	{ .op = 'z', .arg = 10 },
	{ .op = 'w', .arg = 0 },
	{ .op = 'z', .arg = 5 },
	{ .op = 'l', .arg = 0 },
	{ .op = 'z', .arg = 3 }
} ;

#define CHILD_CMD_COUNT ((sizeof(child_cmds)/sizeof(struct Cmd)))


static struct Cmd parent_cmds[] = {
	{ .op = 'z', .arg = 6 },
	{ .op = 'e', .arg = 0 },
	{ .op = 'z', .arg = 5 },
	{ .op = 'l', .arg = 0 },
	{ .op = 'z', .arg = 5 },
	{ .op = 'e', .arg = 0 },
	{ .op = 'z', .arg = 5 },
	{ .op = 'n', .arg = 0 },
	{ .op = 'z', .arg = 5 },
	{ .op = 'l', .arg = 0 }
} ;
	
#define PARENT_CMD_COUNT ((sizeof(parent_cmds)/sizeof(struct Cmd)))
