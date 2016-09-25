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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../bmp/bmp.h"
#include "fixer.h"

static	int		do_fix(pbmp_t* p_p_bmp);

int fix_files(list_t file_list)
{
    int fixed_num = 0;
    int total_num = 0;

    plist_node_t p_node = file_list;

    do {
        total_num++;

        char* path = (char*)(p_node->p_item);
        printf("Fixing file \"%s\"...\n", path);

        //Load file
        pbmp_t p_bitmap = load_bitmap(path);

        if(p_bitmap == NULL) {
            fprintf(stderr, "Failed to load \"%s\".\n", path);
            p_node = p_node->p_next;
            continue;
        }

        //Fix file
        if(do_fix(&p_bitmap) != 0) {
            release_bitmap(p_bitmap);
            p_node = p_node->p_next;
            continue;
        }

        //Save file
        //Get backup file name
        char* backup_name = (char*)malloc(strlen(path) + 1 + 4);

        if(backup_name == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            release_bitmap(p_bitmap);
            p_node = p_node->p_next;
            continue;
        }

        strcpy(backup_name, path);
        strcat(backup_name, ".bak");

        //Backup file
        if(rename(path, backup_name) != 0) {
            fprintf(stderr, "Failed to save backup file \"%s\".", backup_name);
            free(backup_name);
            release_bitmap(p_bitmap);
            p_node = p_node->p_next;
            continue;
        }

        //Save file
        if(save_bitmap(path, p_bitmap) != 0) {
            fprintf(stderr, "Failed to save file \"%s\".", path);
            rename(backup_name, path);

        } else {
            fixed_num++;
        }

        free(backup_name);
        release_bitmap(p_bitmap);
        p_node = p_node->p_next;
    } while(p_node != file_list);


    return 0;
}

int do_fix(pbmp_t* p_p_bmp)
{
    (void)(p_p_bmp);

    return 0;
}
