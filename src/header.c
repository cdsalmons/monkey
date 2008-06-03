/*  Monkey HTTP Daemon
 *  ------------------
 *  Copyright (C) 2008, Eduardo Silva P.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "monkey.h"

struct mk_iov *mk_header_iov_create(int n)
{
	struct mk_iov *iov;

	iov = M_malloc(sizeof(struct mk_iov));
	iov->iov_idx = 0;
	iov->io = M_malloc(n*sizeof(struct iovec));
	iov->buf_to_free = M_malloc(n*sizeof(char));
	iov->buf_idx = 0;
	return iov;
}

int mk_header_iov_add_line(struct mk_iov *mk_io, char *buf, int len, int free)
{
	mk_io->io[mk_io->iov_idx].iov_base = buf;
	mk_io->io[mk_io->iov_idx].iov_len = len;
	mk_io->iov_idx++;

	mk_io->io[mk_io->iov_idx].iov_base = RESP_BREAK_LINE;
	mk_io->io[mk_io->iov_idx].iov_len = LEN_RESP_BREAK_LINE;

	if(free==MK_IOV_FREE_BUF)
	{
		mk_io->buf_to_free[mk_io->buf_idx] = (char *)buf;
		mk_io->buf_idx++;
	}
	return mk_io->iov_idx++;
}

ssize_t mk_header_iov_send(int fd, struct mk_iov *mk_io)
{
	ssize_t n;

	//mk_header_iov_add_line(mk_io, RESP_BREAK_LINE, LEN_RESP_BREAK_LINE);
	n = writev(fd, mk_io->io, mk_io->iov_idx);
	return n;
}

void mk_header_iov_free(struct mk_iov *mk_io)
{
	int i;
	for(i=0; i<=mk_io->buf_idx;i++)
	{
		M_free(mk_io->buf_to_free[i]);
	}
	M_free(mk_io->io);
	M_free(mk_io);
}


