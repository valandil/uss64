/*

dlist decoder script for pj64, by shygoo

*/

const RSP_DISPLAYLIST_CMD = 1;

const RSP_COMMAND_ADDR = 0xA4000FC0;
const RSP_UCODE_ENGINE_ADDR = 0xA4000FD0;
const RSP_DISPLAYLIST_ADDR = 0xA4000FF0;
const RSP_DISPLAYLIST_SIZE = 0xA4000FF4;

const UCODE_F3D    = 0x3A1CBAC3;
const UCODE_F3DEX2 = 0xC901CEF3; // from BT

const G_MW_SEGMENT = 6;

// janky, probably not the correct place to hook
var evtdraw = events.ondraw(function()
{
    if(mem.u32[RSP_COMMAND_ADDR] != RSP_DISPLAYLIST_CMD)
    {
        return;
    }

    events.remove(evtdraw);

    var dlistAddr = mem.u32[RSP_DISPLAYLIST_ADDR];
    var dlistSize = mem.u32[RSP_DISPLAYLIST_SIZE];
    var ucodeEngineAddr = mem.u32[RSP_UCODE_ENGINE_ADDR];

    var decoder = new Decoder();

    decoder.setMicrocode(ucodeEngineAddr);
    decoder.setDisplayList(dlistAddr, dlistSize);
    decoder.run();

    var romname = rom.getstring(0x20).trim();

    var path = 'dldump-' + romname + '.txt';

    var fd = fs.open(path, 'wb');
    fs.write(fd, decoder.result);
    fs.close(fd);
    console.log('wrote result to ' + path);
});

function Decoder(dlistAddr)
{
    this.result = null;
    this.segments = null;
    this.stack = null;
    this.stackIndex = 0;
    this.addr = dlistAddr;
    this.ucodeVersion = 0;
}

Decoder.prototype = {
    dl: function(addr)
    {
        if(this.stackIndex == 16)
        {
            throw Error("dlstack overflow");
        }

        this.stack[this.stackIndex++] = this.addr;
        this.addr = addr;
    },
    branchdl: function(addr)
    {
        this.addr = addr;
    },
    enddl: function()
    {
        if(this.stackIndex == -1)
        {
            throw Error("dlstack underflow");
        }

        this.addr = this.stack[--this.stackIndex];
    },
    readcmd: function()
    {
        var vaddr = this.segmentedToVirtual(this.addr);

        var command = {
            word0: mem.u32[vaddr + 0],
            word1: mem.u32[vaddr + 4],
        };

        command.op = (command.word0 >>> 24);

        this.addr += 8;
        return command;
    },
    setMicrocode: function(ucodeEngineAddr)
    {
        var vaddr = 0x80000000 | ucodeEngineAddr;
        var checksum = 0;

        for(var i = 0; i < 3072; i += 4)
        {
            checksum = (checksum + mem.u32[vaddr + i]) & 0xFFFFFFFF;
        }
    
        this.ucodeVersion = checksum;
        console.log("microcode " + checksum.hex())
    },
    setDisplayList: function(dlistAddr, dlistSize)
    {
        console.log("dlist " + dlistAddr.hex())
        this.segments = new Array(16);
        this.stack = new Array(16);
        this.stackIndex = 0;
        this.addr = dlistAddr;
        this.size = dlistSize;

        for(var i = 0; i < 16; i++)
        {
            this.segments[i] = 0;
            this.stack[i] = 0;
        }
    },
    run: function()
    {
        this.result = "";

        switch(this.ucodeVersion)
        {
        case UCODE_F3D:
            this.decodeF3D();
            break;
        case UCODE_F3DEX2:
            this.decodeF3DEX2();
            break;
        default:
            throw Error("error: unhandled ucode (engine: " + ucodeVersion.hex() + ", dlist: " + dlistAddr + ")");
            break;
        }
    },
    segmentedToVirtual: function(addr)
    {
        var segment = (addr >>> 24) & 0x0F;
        var offset = addr & 0x00FFFFFF;
        return 0x80000000 | (this.segments[segment] + offset);
    },
    log: function(str)
    {
        this.result += str;
        if(this.debug == true)
        {
            console.print(str);
        }
    }
}

Decoder.bits = function(value, shift, nbits)
{
    return (value >> shift) & ~(-1 << nbits);
}

function breakdownArgs(command, fields)
{
    var fieldStrs = {};

    for(var fieldName in fields)
    {
        var fieldInfo = fields[fieldName];
        var word = (fieldInfo.word == 0) ? command.word0 : command.word1;
        var shift = fieldInfo.sh || 0;
        var bits = fieldInfo.bits || 32;
        var mask = ~(-1 << bits) || 0xFFFFFFFF;

        var fieldValue = (word >>> shift) & mask;
        
        var fieldStr = null;

        if(fieldInfo.denum)
        {
            if(typeof(fieldInfo.denum) == 'function')
            {
                fieldStr = fieldInfo.denum(fieldValue);
            }
            else
            {
                fieldStr = fieldInfo.denum[fieldValue];
            }
        }

        if(!fieldStr)
        {
            fieldStr = fieldValue;
        }

        //fieldStrs.push(fieldStr);
        fieldStrs[fieldName] = fieldStr;
    }
    //console.log(JSON.stringify(fieldStrs));
    //console.log(fieldStrs.join(', '));

    return JSON.stringify(fieldStrs);
}

function dec_hex(n)
{
    return n.hex();
}

var FIELD_INFO_UC0_MOVEWD = {
    'segment': { word: 0, sh: 10, bits: 4 },
    'index': { word: 0, sh: 0, bits: 8, denum: { 6: 'G_MW_SEGMENT' } },
    'param': { word: 1, denum: dec_hex }
}

Decoder.prototype.decodeF3D = function()
{
    //console.log("fast3d 1.0");

    var num = 0;

    while(this.stackIndex >= 0)
    {
        this.log(this.addr.hex() + ": ");
        var command = this.readcmd();

        for(var i = 0; i < this.stackIndex; i++)
        {
            this.log("    ");
        }
        
        this.log(command.word0.hex() + " " + command.word1.hex() + "\r\n");

        switch(command.op)
        {
        case 0x00: // nop
            throw Error('hit a nop?');
            break;
        case 0xBC: // gsMoveWd
            var addr = command.word1;
            var segment = Decoder.bits(command.word0, 10, 4);
            var wordIdx = Decoder.bits(command.word0, 0, 8);

            if(wordIdx == G_MW_SEGMENT)
            {
                this.segments[segment] = addr;
            }

            //this.result += breakdownArgs(command, FIELD_INFO_UC0_MOVEWD) + "\r\n";

            break;
        case 0x06: // gsDisplayList
            var branch = Decoder.bits(command.word0, 16, 8);
            if(branch == 1)
            {
                this.branchdl(command.word1);
            }
            else
            {
                this.dl(command.word1);
            }
            break;
        case 0xB8: // gsEndDisplayList
            this.enddl();
            break;
        }
    }
}