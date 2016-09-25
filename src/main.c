/*
   Copyright 2016,王思远 <darknightghost.cn@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <errno.h>

#include "list/list.h"
#include "fixer/fixer.h"

#if defined(WINDOWS)
    #include <windows.h>
#elif defined(LINUX)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

static	void	usage(char* name);
static	list_t	get_file_list(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    //Analyse parameters
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    list_t file_list = get_file_list(argc, argv);

    if(file_list == NULL) {
        return -1;
    }

    //Repair screenshots
    int ret = fix_files(file_list);
    list_destroy(&file_list, NULL);

    return ret;
}

void usage(char* name)
{
    printf("Usage:\n"
           "\t%s path1 path2 ...\n\n",
           name);

    return;
}

list_t get_file_list(int argc, char* argv[])
{
    list_t ret;

    list_init(ret);

    for(int i = 1; i < argc; i++) {

        //Test if the file exists
        #if defined(WINDOWS)
        DWORD attr = GetFileAttributes(argv[i]);

        if(attr == INVALID_FILE_ATTRIBUTES) {
            errno = ENOENT;
            fprintf(stderr, "\"%s\" does not exist.\n", argv[i]);
            goto _ERR;

        } else if(attr | FILE_ATTRIBUTE_DIRECTORY) {
            errno = EISDIR;
            fprintf(stderr, "\"%s\" is a directory.\n", argv[i]);
            goto _ERR;
        }

        #elif defined(LINUX)
        struct stat st;

        if(stat(argv[i], &st) != 0) {
            errno = ENOENT;
            fprintf(stderr, "\"%s\" does not exist.\n", argv[i]);
            goto _ERR;

        } else if(S_ISDIR(st.st_mode)) {
            errno = EISDIR;
            fprintf(stderr, "\"%s\" is a directory.\n", argv[i]);
            goto _ERR;
        }

        #endif

        if(list_insert_after(&ret, NULL, argv[i]) == NULL) {
            errno = ENOMEM;
            perror("Failed to allocate memory!\n");
            goto _ERR;
        }

    }

    return ret;

_ERR:
    list_destroy(&ret, NULL);
    return NULL;
}
