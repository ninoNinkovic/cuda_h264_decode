#include <string>
#include <iostream>
#include <stdint.h>

#include "inc/constants.h"
#include "inc/RBSP_structs.h"
#include "inc/types.h"
#include "inc/classes.h"

using std::cout;
using std::endl;
using std::string;

// initialize to NULL byte with MSB
BitPos::BitPos(): byte( NULL ), mask( 0x80 ) { }

// initialize to MSB of given byte
BitPos::BitPos( uint8_t* in_byte ): byte( in_byte ), mask( 0x80 ) { }

// initialize with given position info
BitPos::BitPos( uint8_t* in_byte, uint8_t in_mask ): byte( in_byte ), mask( in_mask ) { }

// getters and setters for member values
uint8_t* BitPos::getByte( void )
{ return byte; }
void BitPos::setByte( uint8_t* in_byte )
{ byte = in_byte; }

uint8_t BitPos::getMask( void )
{ return mask; }
void BitPos::setMask( uint8_t in_mask )
{ mask = in_mask; }

// walk forward one bit in the stream independent of byte borders
void BitPos::advance( void )
{
	mask >>= 1;
	if( !mask ) { ++byte; mask = 0x80; }
}

// same as advance, but in the opposite direction
void BitPos::retreat( void )
{
	mask <<= 1;
	if( !mask ) { --byte; mask = 0x01; }
}

// read numBits bits starting at current position independent of byte borders
uint32_t BitPos::readBits( int numBits )
{
	uint32_t retVal = 0;

	if( 8 == numBits && 0x80 == mask ) // optimize an aligned byte read
	{
		retVal = *byte;
		++byte;

		return retVal;
	}

	retVal |= ( *byte & mask ) ? 1 : 0;
	advance( );

	numBits -= 1;

	for( int i = 0; i < numBits; ++i )
	{
		retVal <<= 1;

		retVal += ( *byte & mask ) ? 1 : 0;
		advance( );
	}

	return retVal;
}

// use this if byte-alignment is required
uint8_t BitPos::readByte( void )
{
	uint8_t retVal = 0;

	mask = 0x80;

	retVal = *byte;
	++byte;

	return retVal;
}

// read one bit and leave the position "behind" the bit read
bool BitPos::readBitReverse( void )
{
	bool retVal = false;

	retVal = ( *byte & mask ) ? 1 : 0;
	retreat( );

	return retVal;
}

// destructor not needed, must be declared
BitPos::~BitPos( ) { }
