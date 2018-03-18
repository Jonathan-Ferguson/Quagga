/*
 * Exercise thread_execute.  Exercise that lib/thread copes with poor usage
 * by callers.
 *
 * Copyright © 2018 Paul Jakma <paul@jakma.org>
 *
 * This file is part of Quagga
 *
 * Quagga is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * Quagga is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quagga; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <zebra.h>
#include "thread.h"

#include <stdio.h>

static int
execf (struct thread *t)
{
  int *times = t->arg;
  
  printf ("%s: called, %u\n", __func__, *times);
  
  *times -= 1;
  
  if (*times <= 0)
    exit (0);
  
  thread_cancel (t);
  
  return 0;
}

static int
timerf (struct thread *t)
{
  thread_execute (t->master, execf, t->arg, 0); 
  thread_add_timer_msec (t->master, timerf, t->arg, 10);
  return 0;
}

int main(int argc, char *argv[])
{
  int times = 10;
  struct thread_master *m = thread_master_create ();
  thread_add_timer_msec (m, timerf, &times, 10);
  
  thread_main (m);
  
  return 0;
}
