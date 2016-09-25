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

#pragma once
#include <stdlib.h>

typedef	unsigned char	u8;
typedef	unsigned short	u16;
typedef	unsigned int	u32;
typedef	signed int		s32;

typedef struct {
    u16		type;
    u32		size;
    u16		rsvd1;
    u16		rsvd2;
    u32		offbits;
} __attribute__((packed)) bitmap_file_header_t, *pbitmap_file_header_t;

typedef struct {
    u32		size;
    u32		width;
    s32		height;
    u16		planes;
    u16		bit_count;
    u32		compression;
    u32		size_image;
    u32		x_pels_per_meter;
    u32		y_pels_per_meter;
    u32		clr_used;
    u32		clr_important;
} __attribute__((packed)) bitmap_info_header_t, *pbitmap_info_header_t;

typedef struct {
    u8	red;
    u8	green;
    u8	blue;
    u8	flags;
} __attribute__((packed)) bitmap_palette_t, *pbitmap_palette_t;

typedef	struct {
    bitmap_file_header_t	file_header;
    bitmap_info_header_t	info_header;
    u8						data[];
} __attribute__((packed)) bmp_file_t, *pbmp_file_t;

typedef struct {
    size_t		size;
    bmp_file_t	data;
} __attribute__((packed)) bmp_t, *pbmp_t;

pbmp_t load_bitmap(char* path);
int save_bitmap(char* path, pbmp_t p_bitmap);
pbmp_t new_bitmap(size_t size);
void release_bitmap(pbmp_t p_bitmap);
