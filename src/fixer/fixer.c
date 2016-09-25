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

        printf("\n");

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
        remove(backup_name);

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

    printf("%d file(s) in total. %d file(s) fixed.\n",
           total_num, fixed_num);

    return 0;
}

int do_fix(pbmp_t* p_p_bmp)
{
    pbmp_t p_src_bmp = *p_p_bmp;

    //Check file format
    if(p_src_bmp->data.info_header.compression != 0) {
        fprintf(stderr, "Unsupported compression type!\n");
        return -1;

    } else {
        printf("Compression type: Uncompressed.\n");
    }

    printf("Bitcount : %u.\n", p_src_bmp->data.info_header.bit_count);

    if(p_src_bmp->data.info_header.bit_count != 24) {
        fprintf(stderr, "Unsupported bit count!\n");
        return -1;
    }

    u32 width = p_src_bmp->data.info_header.width;
    u32 height = (u32)(p_src_bmp->data.info_header.height);

    printf("Picture size : %u * %u.\n", width, height);

    printf("File size : %u.\n"
           "File size in bitmap header : %u.\n",
           (u32)(p_src_bmp->size),
           p_src_bmp->data.file_header.size);

    //Check if the file needs repare
    size_t data_size = width * 3;

    if(data_size % 4 != 0) {
        data_size += 4 - data_size % 4;
    }

    data_size *= height;
    size_t file_size = data_size + sizeof(bitmap_file_header_t)
                       + sizeof(bitmap_info_header_t);
    printf("Correct file size : %u.\n", (u32)file_size);

    if(p_src_bmp->data.file_header.size >= file_size) {
        printf("The file does not need repair.\n");
        return -1;

    }

    //Fix the bitmap
    pbmp_t p_dest_bmp = new_bitmap(file_size);

    if(p_dest_bmp == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return - 1;
    }

    memcpy(&(p_dest_bmp->data), &(p_src_bmp->data), sizeof(bitmap_file_header_t)
           + sizeof(bitmap_info_header_t));
    p_dest_bmp->data.file_header.size = file_size;

    size_t src_line_sz = width * 3;
    size_t dest_line_sz = src_line_sz;

    if(dest_line_sz % 4 != 0) {
        dest_line_sz += 4 - dest_line_sz % 4;
    }

    u8* p_src = p_src_bmp->data.data;
    u8* p_dest = p_dest_bmp->data.data;

    for(int i = 0; i < height; i++) {
        memcpy(p_dest, p_src, src_line_sz);
        p_src += src_line_sz;
        p_dest += dest_line_sz;
    }

    release_bitmap(p_src_bmp);

    *p_p_bmp = p_dest_bmp;

    return 0;
}
