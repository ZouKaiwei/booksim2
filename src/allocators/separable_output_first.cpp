// ----------------------------------------------------------------------
//
//  SeparableOutputFirstAllocator: Separable Output-First Allocator
//
// ----------------------------------------------------------------------

#include "separable_output_first.hpp"

#include "booksim.hpp"
#include "arbiter.hpp"

#include <vector>
#include <iostream>
#include <string.h>

SeparableOutputFirstAllocator::
SeparableOutputFirstAllocator( const Configuration& config,
			       Module* parent, const string& name,
			       const string& arb_type,
			       int inputs, int outputs )
  : SeparableAllocator( config, parent, name, arb_type, inputs, outputs )
{}

void SeparableOutputFirstAllocator::Allocate() {
  
  _ClearMatching() ;

//  cout << "SeparableOutputFirstAllocator::Allocate()" << endl ;
//  PrintRequests() ;
  
  list<sRequest> out_reqs[_outputs];
  
  for ( int input = 0 ; input < _inputs ; input++ ) {
    
    // Add requests to the output arbiters
    list<sRequest>::const_iterator it  = _requests[input].begin() ;
    list<sRequest>::const_iterator end = _requests[input].end() ;
    while ( it != end ) {
      const sRequest& req = *it ;
      if ( req.label > -1 ) {
	_output_arb[req.port]->AddRequest( input, req.label, req.out_pri );
      }
      it++ ;
    }
    
  }
  
  for ( int output = 0; output < _outputs; output++ ) {
    
    // Execute the output arbiters and propagate the grants to the
    // input arbiters.
    int label, pri ;
    int in = _output_arb[output]->Arbitrate( &label, &pri ) ;
    
    if ( in > -1 ) {
      _input_arb[in]->AddRequest( output, label, pri ) ;
    }
  }
  
  // Execute the input arbiters.
  for ( int input = 0 ; input < _inputs ; input++ ) {

    int label, pri ;
    int output      = _input_arb[input]->Arbitrate( &label, &pri ) ;
  
    if ( output > -1 ) {
      assert( _inmatch[input] == -1 && _outmatch[output] == -1 ) ;
      _inmatch[input]   = output ;
      _outmatch[output] = input ;
      _input_arb[input]->UpdateState() ;
      _output_arb[output]->UpdateState() ;

    }
  }
}
