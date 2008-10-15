// ----------------------------------------------------------------------
//
//  SeparableInputFirstAllocator: Separable Input-First Allocator
//
// ----------------------------------------------------------------------

#include "separable_input_first.hpp"

#include "booksim.hpp"
#include "arbiter.hpp"

#include <vector>
#include <iostream>
#include <string.h>

SeparableInputFirstAllocator::
SeparableInputFirstAllocator( const Configuration& config,
			      Module* parent, const string& name,
			      const string& arb_type,
			      int inputs, int outputs )
  : SeparableAllocator( config, parent, name, arb_type, inputs, outputs )
{}

void SeparableInputFirstAllocator::Allocate() {
  
  _ClearMatching() ;

//  cout << "SeparableInputFirstAllocator::Allocate()" << endl ;
//  PrintRequests() ;

  for ( int input = 0 ; input < _inputs ; input++ ) {
   
    // Add requests to the input arbiters.
    list<sRequest>::const_iterator it  = _requests[input].begin() ;
    list<sRequest>::const_iterator end = _requests[input].end() ;
    while ( it != end ) {
      const sRequest& req = *it ;
      if ( req.label > -1 ) {
	_input_arb[input]->AddRequest( req.port, req.label, req.in_pri ) ;
      }
      it++ ;
    }

    // Execute the input arbiters and propagate the grants to the
    // output arbiters.
    int label, pri ;
    int out = _input_arb[input]->Arbitrate( &label, &pri ) ;
    
    if ( out > -1 ) {
      _output_arb[out]->AddRequest( input, label, pri ) ;
    }
  }

  // Execute the output arbiters.
  for ( int output = 0 ; output < _outputs ; output++ ) {

    int label, pri ;
    int input      = _output_arb[output]->Arbitrate( &label, &pri ) ;
  
    if ( input > -1 ) {
      assert( _inmatch[input] == -1 && _outmatch[output] == -1 ) ;
      _inmatch[input]   = output ;
      _outmatch[output] = input ;
      _input_arb[input]->UpdateState() ;
      _output_arb[output]->UpdateState() ;

    }
  }
}
