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

#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>

pbmp_t load_bitmap(char* path)
{
    FILE* fp = fopen(path, "rb");

    if(fp == NULL) {
        fprintf(stderr, "Failed to open file \"%s\".\n", path);
        return NULL;
    }

    //Get size
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp) + 1;

    if(size <= sizeof(bitmap_file_header_t) + sizeof(bitmap_info_header_t)) {
        fclose(fp);
        fprintf(stderr, "Illegal file format \"%s\".\n", path);
        return NULL;
    }

    rewind(fp);

    //Read file
    pbmp_t buf = (pbmp_t)malloc(size + sizeof(size_t));
    buf->size = size;
    fread(&(buf->data), 1, size, fp);
    fclose(fp);

    //Check file
    if(buf->data.file_header.type != *(u16*)"BM"
       || buf->data.file_header.rsvd1 != 0
       || buf->data.file_header.rsvd2 != 0) {
        fprintf(stderr, "Illegal file format \"%s\".\n", path);
        free(buf);
        return NULL;
    }

    return buf;
}

int save_bitmap(char* path, pbmp_t p_bitmap)
{
    FILE* fp = fopen(path, "wb");

    fwrite(&(p_bitmap->data), 1, p_bitmap->size, fp);

    fclose(fp);

    return 0;
}

pbmp_t new_bitmap(size_t size)
{
    pbmp_t p_ret = (pbmp_t)malloc(size);
    p_ret->size = size;
    p_ret->data.file_header.size = size;
    return p_ret;
}

void release_bitmap(pbmp_t p_bitmap)
{
    free(p_bitmap);
    return;
}
