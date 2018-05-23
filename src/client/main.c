//
// Created by snipy on 21.03.18.
// Update client tcp/ip by MP on 22.03.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#include "../utils/debug.h"
#include "shell.h"
#include "../../lib/argparser/src/argparser.h"
#include "commands_executor.h"

int main(int argc, char* argv[])
{
    args_init(
            "A TCP client to connect to a key-value database",
            "client [options] <server-ip-address>",
            "This program is free software: you can redistribute it and/or modify\n"
            "    it under the terms of the GNU General Public License as published by\n"
            "    the Free Software Foundation, either version 3 of the License, or\n"
            "    (at your option) any later version.\n"
            "\n"
            "    This program is distributed in the hope that it will be useful,\n"
            "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "    GNU General Public License for more details.\n"
            "\n"
            "    You should have received a copy of the GNU General Public License\n"
            "    along with this program.  If not, see <http://www.gnu.org/licenses/>."
            "",
            0, 1, 0, args_standard_abbreviations
    );

    args_add_option(args_new_argumentless(NULL, 1, "-h", "-?", "--help", NULL),
                    "Prints this help message\n(and exits)");
    args_add_option(args_new_argumented(NULL, "FILE", 0, "-f", "--file", NULL),
                    "read a file a execute the command from it");
    args_add_option(args_new_argumented(NULL, "FILE", 0, "-l", "--logfile", NULL),
                    "specify the name of the logfile");

    args_add_option(args_new_variadic(NULL, "LINE", 0, "-F", "--files", NULL),
                    "read multiple files a execute the commands from it");

    args_parse(argc, argv);
    args_support_alternatives();

    if (args_opts_used("-?"))
    {
        args_help();
    }
    else if (args_unrecognised_count < 1 && args_arguments_count > 0)
    {
        int socket_fd = 0;
        char receive_buffer[1024];
        struct sockaddr_in server_addr;

        if (args_files_count != 1)
        {
            log_err("\nclient [options] <server-ip-address>\n");
            return 1;
        }

        const char* server_ip = args_files[args_files_count - 1]; // ip address

        memset(receive_buffer, '0', sizeof(receive_buffer));
        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            log_err("\n Error : Could not create socket \n");
            return 1;
        }

        memset(&server_addr, '\0', sizeof(server_addr));

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000);

        if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
        {
            log_err("Errno error : %s\n", strerror(errno));
            log_err("\n inet_pton error occurred\n");
            return 1;
        }

        if (connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
        {
            log_err("Connection failed : %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        if (args_opts_used("-f"))
        {
            size_t nb_file = args_opts_get_count("-f");
            char** files = args_opts_get("-f");

            size_t nb_log_file = args_opts_get_count("-l");
            char** log_files = args_opts_get("-l");

            for (int j = 0; j < nb_file; j++)
            {
                log_info("Process command file %s : ", files[j]);
                int status = process_command_file(files[j], log_files[j], socket_fd);
                if (status == EXIT_FAILURE)
                {
                    // TODO
                }
            }
        }
        else
        {
            shell_loop(socket_fd);
        }
    }
    else
    {
        size_t i;
        printf("Number of unrecognised options: %li\n", args_unrecognised_count);
        printf("Entered message: %s\n", args_message ? args_message : "null");
        printf("Entered files:\n");
        for (i = 0; i < args_files_count; i++)
            printf("\t%s\n", *(args_files + i));
    }

    return EXIT_SUCCESS;
}