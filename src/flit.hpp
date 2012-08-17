// $Id$

/*
 Copyright (c) 2007-2011, Trustees of The Leland Stanford Junior University
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this 
 list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FLIT_HPP_
#define _FLIT_HPP_

#include <iostream>
#include <stack>
#include "reservation.hpp"
#include "booksim.hpp"

class Flit {

public:
  bool inuse;

  const static int NUM_FLIT_TYPES = 5;
  enum FlitType { READ_REQUEST  = 0, 
		  READ_REPLY    = 1,
		  WRITE_REQUEST = 2,
		  WRITE_REPLY   = 3,
                  ANY_TYPE      = 4 };
  FlitType type;

  SRPFlitType res_type;
  int exptime;
  int  sn;
  int flid;
  int payload; 
  //res = reservation size
  //grant = time
  //tail = tail reservation
  //body = expected arrival time

  short packet_size;
  //head: packet valid
  //body: compression 

  int head_sn;

  int flbid;

  bool walkin;

  bool fecn;
  bool becn;

  short vc;
  short cl;

  bool head;
  bool tail;

  int ntime;
  int  time;
  //  int  ttime;
  int  atime;

  //  int  rob_time;

  int  id;
  int  pid;
  //  int  tid;

  bool record;

  short  src;
  short  dest;

  int  pri;

  short  hops;
  bool watch;
  short  subnetwork;

  // Fields for multi-phase algorithms
  mutable short intm;
  mutable short ph;

  //mutable int dr;
  mutable short minimal; // == 1 minimal routing, == 0, nonminimal routing

  // Which VC parition to use for deadlock avoidance in a ring
  //mutable int ring_par;

  // Fields for arbitrary data
  //  void* data ;

  void Reset();

  static Flit * Replicate( Flit *);
  static Flit * New();
  void Free();
  static void FreeAll();

  static int Allocated(){return _all.size();};
  static int OutStanding();
  
private:

  Flit();
  ~Flit() {}

  static stack<Flit *> _all;
  static stack<Flit *> _free;

};

ostream& operator<<( ostream& os, const Flit& f );

#endif
