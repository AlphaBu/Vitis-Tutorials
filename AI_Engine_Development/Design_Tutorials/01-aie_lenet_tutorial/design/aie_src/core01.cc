/* (c) Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <stdlib.h>
#include <stdio.h>

#include "core01.h"
#include "core01lut.h"

#ifndef INLINE
INLINE_DECL void core01(
        const int RowA_tile,
        const int ColA_tile,
        const int ColB_tile,
        int8_t* A_in,
        //int8_t* B_in,
        int8_t* C_out,
        int shift
) {      
	v32int8 chess_storage(DM_bankA)* restrict vA1 = (v32int8 chess_storage(DM_bankA)*) A_in;
	v32int8 chess_storage(DM_bankA)* restrict vA2 = vA1 + ColA_tile;

	v32int8 chess_storage(DM_bankA)* restrict vB = (v32int8 chess_storage(DM_bankA)*) B;
	
	v16int8* restrict vC1 = (v16int8*) C_out;
	v16int8* restrict vC2 = vC1 + 2*ColB_tile;
	
	

	v32int8  chess_storage(wc1) Abuff1;
	v32int8  chess_storage(wc0) Abuff0;
	
	
	v128int8 chess_storage(ya) Bbuff = undef_v128int8();
	
	v16acc48 acc0; 
	v16acc48 acc1;
	v16acc48 acc2; 
	v16acc48 acc3;

	

    int i,j,z;
	
	for (z=0; z<RowA_tile; z++)
	chess_loop_range(2,)

	{

		for (j=0; j<ColB_tile; j++)
        chess_loop_range(2,)

		{		   	  	
		   	Abuff0 = *vA1++;                                        //[00:31]
		   	Bbuff = upd_w(Bbuff, 0, *vB++);	                        //[00:31|XX:XX|XX:XX|XX:XX]	   		   			   	
			//     mul16(xbuff,xstart,xoffset,xstep,xsquare,zbuff,zstart,zoffsets,zstep,zsquare)
		  	acc0 = mul16(Bbuff, 0,  0x00000000, 8, 0x3120, Abuff0, 0, 0xCC884400, 2, 0x3210);
		  	
            Bbuff = upd_w(Bbuff, 1, *vB); vB += 2*ColB_tile-1;      //[00:31|32:63|XX:XX|XX:XX]		   	
		  	acc1 = mul16(Bbuff, 32, 0x00000000, 8, 0x3120, Abuff0, 0, 0xCC884400, 2, 0x3210);
		  	
		   	Abuff1 = *vA2++;                                        //[32:63]	   			   	
		  	acc2 = mul16(Bbuff, 0,  0x00000000, 8, 0x3120, Abuff1, 0, 0xCC884400, 2, 0x3210); 	
		  	acc3 = mul16(Bbuff, 32, 0x00000000, 8, 0x3120, Abuff1, 0, 0xCC884400, 2, 0x3210);
		  	
		  	
            for (i=0; i<ColA_tile-1; i++)
            chess_prepare_for_pipelining
			chess_loop_range(3,)
		
			{
              vB = chess_copy(vB);        // See CRVO-1587  	 
              
              Abuff0 = *vA1++;                                        //[00:31]
              Bbuff = upd_w(Bbuff, 0, *vB++);	                    //[00:31|XX:XX|XX:XX|XX:XX]	   		   			   	
              acc0 = mac16(acc0, Bbuff, 0,  0x00000000, 8, 0x3120, Abuff0, 0, 0xCC884400, 2, 0x3210);
              
              Bbuff = upd_w(Bbuff, 1, *vB); vB += 2*ColB_tile-1;      //[00:31|32:63|XX:XX|XX:XX]		   	
              acc1 = mac16(acc1, Bbuff, 32, 0x00000000, 8, 0x3120, Abuff0, 0, 0xCC884400, 2, 0x3210);
                            
              Abuff1 = *vA2++;                                        //[32:63]	   			   	
              acc2 = mac16(acc2, Bbuff, 0,  0x00000000, 8, 0x3120, Abuff1, 0, 0xCC884400, 2, 0x3210); 	
              acc3 = mac16(acc3, Bbuff, 32, 0x00000000, 8, 0x3120, Abuff1, 0, 0xCC884400, 2, 0x3210);
			}
			
            //will require rearrangement of output                        
            *vC1++ = bsrs(acc0,shift);
            *vC1++ = bsrs(acc1,shift);
            *vC2++ = bsrs(acc2,shift);
            *vC2++ = bsrs(acc3,shift);

                        			
            vB  += 2 - 2*ColB_tile*ColA_tile;
			vA1 += -ColA_tile;
			vA2 += -ColA_tile;


			
		}
		
		
		vA1 += 2*ColA_tile;
   		vA2 += 2*ColA_tile;
		vB = (v32int8 chess_storage(DM_bankA)*) B;
		vC1 += 2*ColB_tile;
		vC2 += 2*ColB_tile;
	}
}
#endif

void core01_top(input_window_int32 *inA, output_window_int32 *out){
	int shift = 9;
    set_sat();
    set_rnd(rnd_sym_inf);
	core01(ROW_A >> 3, COL_A >> 3, COL_B >> 3, (int8_t *) inA -> ptr, (int8_t *) out -> ptr, shift);
}
