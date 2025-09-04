int
match_secret_command(const char *passwd)
{
	int i;

	for (i = 0; i < num_secret_commands; i++) {

		/* Safeguard against misconfiguration, i.e. leaving password or command empty */
		if (!secret_commands[i].password || !secret_commands[i].command)
			continue;

		if (!strcmp(secret_commands[i].password, passwd)) {
			system(secret_commands[i].command);
			return 1;
		}
	}

	return 0;
}