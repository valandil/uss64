// ------------------------------------------------------------------------- //
// Author:      Joey Dumont                     <joey.dumont@gmail.com>      //
// Date:        2018-05-28                                                   //
// Description: Break on the call to gfx_flush() in order to debug the       //
//              display list.                                                //
// ------------------------------------------------------------------------- //
events.onexec({USS64_gfx_flush}, function()
{
    // Break if uss64_ready is True.
    if (mem.u32[{USS64_Ready}] == 0x01000000)
    {
        debug.breakhere();
        var addr = mem.u32[{USS64_gfx_disp}]
        var dl_hi,dl_lo;
        do {
            dl_hi = mem.u32[addr];
            dl_lo = mem.u32[addr+4];
            console.log(dl_hi.hex(), dl_lo.hex());
        }
        while (dl_hi != 0xB8000000);
    }
})