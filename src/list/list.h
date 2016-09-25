/*
	  Copyright 2016,暗夜幽灵 <darknightghost.cn@gmail.com>

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

typedef	void	(*destroier)(void*);

typedef	struct	_list_node {
    struct	_list_node*		p_prev;
    struct	_list_node*		p_next;
    void*					p_item;
} list_node_t, *plist_node_t, *list_t, **plist_t;

#define	list_init(x)	((x) = NULL)
plist_node_t	list_insert_before(plist_t p_list, plist_node_t p_position,
                                   void* p_item);
plist_node_t	list_insert_after(plist_t p_list, plist_node_t p_position,
                                  void* p_item);
void			list_remove(plist_t p_list, plist_node_t p_node);
void			list_destroy(plist_t p_list, destroier destroy_func);
