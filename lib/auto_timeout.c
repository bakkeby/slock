
void *
timeout_command(void *args)
{
	int runflag = 0;
	while (!runflag) {
		sleep(auto_timeout_offset);
		runflag = auto_timeout_run_once;
		system(auto_timeout_command);
	}
	return args;
}
