# This is a tool to be able to upload a new
#   application to the target microcontroller
import serial
import time
from serial import SerialException

MAX_ARG_LEN = 4

IDX_COMM = 0

BINARY_PATH = 'C://Users//avasquez//Documents//Atmel Studio//7.0//SampleBootloadableProject//SampleBootloadableProject//Debug//SampleBootloadableProject.bin'

ser = None
out_binary = bytearray()
out_binary_str = ''
bin_size = 0

print "Python SAM-BA bootloader... "

in_arg = raw_input('>> ')
in_arg = str(in_arg)    
while (in_arg.lower() != 'exit'):
    arg_set = in_arg.split()
    if len(arg_set) <= MAX_ARG_LEN:
        if arg_set[IDX_COMM].lower() == 'readbyte' and len(arg_set) == 2:
            if ser != None:
                int_address = int(arg_set[1].lower(),16)
                var_address = "w" + format(int_address, '08x') + ",#"
                ser.write(''.join(x for x in var_address))
                dev_data = ser.read(4)
                print ''.join(format(x, '02x') for x in bytearray(dev_data))
            else:
                print "Port is not connected"
        elif arg_set[IDX_COMM].lower() == 'loadbinary' and len(arg_set) == 1:
            try:
                bin_file = open(BINARY_PATH, 'rb')
                byte = bin_file.read(1)
                bin_size = 0
                while byte != "":
                    bin_size += 1
                    out_binary.append(ord(byte))
                    out_binary_str += format(out_binary[len(out_binary) - 1], '02x')
                    #if bin_size > 1 and bin_size % 4 == 0:
                    #    out_binary_str += ' '
                    #if bin_size > 1 and bin_size % 32 == 0:
                    #    out_binary_str += '\n'
                    byte = bin_file.read(1)
                print 'File loaded! ' + str(bin_size) + ' bytes \n' #+ out_binary_str
            except:
                print "ERROR: File did not load"
        elif arg_set[IDX_COMM].lower() == 'flash' and len(arg_set) == 2:
            #if ser != None:
            if bin_size > 0 and ser != None:
                try:
                    int_address = int(arg_set[1].lower(),16)
                    try:
                        var_address = "S" + format(int_address, '08x') + ",#"
                        #ser.write(''.join(x for x in var_address))
                        #ser.write(''.join(x for x in out_binary_str))
                        #ser.write(b',#')
                        #Proceed with the math to find out the memory headers and sizes
                        base_address = (int_address / 0x40) * 0x40
                        num_round = (base_address + 0x40) - int_address
                        if (int_address % 0x40) > 0:
                            rem_writes = bin_size - num_round
                            num_writes = (rem_writes / 0x40) + 1
                            if (rem_writes % 0x40) > 0:
                                num_writes += 1
                        else:
                            num_writes = bin_size / 0x40
                            if (bin_size % 0x40) > 0:
                                num_writes += 1
                        rem_writes = bin_size
                        num_total = 0
                        base_address = int_address
                        data_send_index = 0
                        byte_checker = 0
                        byte_verifier = 0
                        is_fatal_error = False
                        for it in range(0,num_writes):
                            if is_fatal_error:
                                break
                            num_round = 0
                            str_command = ""
                            if it == 0 and (base_address % 0x40) > 0:
                                base_address = (int_address / 0x40) * 0x40
                                num_round = (base_address + 0x40) - int_address
                                rem_writes -= num_round
                                num_total += num_round
                                str_command += "S" + format(int_address, "08x") + "," + format(num_round, "02x") + "#"
                                ser.write(''.join(x for x in str_command))
                                time.sleep(0.005)
                                #print str_command
                                str_command = ""
                                raw_packet_bytes = bytearray()
                                for byt in range(0,num_round):
                                    raw_packet_bytes.append(out_binary[data_send_index])
                                    data_send_index += 1
                                ser.write(raw_packet_bytes)
                                for sample_it in range(0,len(raw_packet_bytes)):
                                    str_command += format(raw_packet_bytes[sample_it], "02x")
                                    if ((sample_it + 1) % 4) == 0:
                                        str_command += " "
                                    if ((sample_it + 1) % 16) == 0:
                                        str_command += "\n"
                                print str_command
                                str_command = ""
                                time.sleep(0.010)
                                #TODO: Send on Serial
                                    #if (((byt + 1) % 16) == 0) and byt > 0:
                                    #    str_command += "\n"
                                #print str_command
                                #print str(it+1) + " -> " + format(int_address, "08x") + " - " + str(num_round) + " bytes - " + str(num_total) + " bytes"
                                #At this point, we check every single byte that has been sent for verification purposes
                                str_command = ""
                                byte_checker = int_address
                                for byt in range(0,num_round/4):
                                    if is_fatal_error:
                                        break
                                    str_command += "w" + format(byte_checker, "08x") + ",#"
                                    byte_checker += 4
                                    #TODO: Send on Serial!
                                    ser.write(''.join(x for x in str_command))
                                    #print str_command
                                    str_command = ""
                                    dev_data = None
                                    dev_data = bytearray()
                                    dev_data = ser.read(4)
                                    #dev_data = [0, 0, 0, 0]
                                    for byt_v in range(0,4):
                                        #dev_data[byt_v] = out_binary[byte_verifier]
                                        if out_binary[byte_verifier] != ord(dev_data[byt_v]):
                                            print "Did not verify:: Source: " + format(out_binary[byte_verifier], "02x") + ", Target: " + format(ord(dev_data[byt_v]), "02x") + " @" + format(byte_checker-4, "08x")
                                            #print "DID NOT VERIFY"
                                            is_fatal_error = True
                                            break
                                        byte_verifier += 1
                            else:
                                if (rem_writes / 0x40) > 0:
                                    rem_writes -= 64
                                    num_round = 64
                                else:
                                    num_round = rem_writes
                                num_total += num_round
                                str_command += "S" + format((0x40*it)+base_address, "08x") + "," + format(num_round, "02x") + "#"
                                ser.write(''.join(x for x in str_command))
                                time.sleep(0.005)
                                #print str_command
                                str_command = ""
                                raw_packet_bytes = bytearray()
                                for byt in range(0,num_round):
                                    raw_packet_bytes.append(out_binary[data_send_index])
                                    data_send_index += 1
                                ser.write(raw_packet_bytes)
                                for sample_it in range(0,len(raw_packet_bytes)):
                                    str_command += format(raw_packet_bytes[sample_it], "02x")
                                    if ((sample_it + 1) % 4) == 0:
                                        str_command += " "
                                    if ((sample_it + 1) % 16) == 0:
                                        str_command += "\n"
                                print str_command
                                str_command = ""
                                time.sleep(0.010)
                                # TODO: Send on serial!
                                    #if (((byt + 1) % 16) == 0) and byt > 0:
                                    #    str_command += "\n"
                                #print str_command
                                #print str(it+1) + " -> " + format((0x40*it)+base_address, "08x") + " - " + str(num_round) + " bytes - " + str(num_total) + " bytes"
                                #At this point, we check every single byte that has been sent for verification purposes
                                str_command = ""
                                if byte_checker == 0:
                                    byte_checker = int_address
                                for byt in range(0,num_round/4):
                                    if is_fatal_error:
                                        break
                                    str_command += "w" + format(byte_checker, "08x") + ",#"
                                    byte_checker += 4
                                    ser.write(''.join(x for x in str_command))
                                    #print str_command
                                    # TODO: Send on Serial!
                                    str_command = ""
                                    dev_data = None
                                    dev_data = bytearray()
                                    dev_data = ser.read(4)
                                    #dev_data = [0, 0, 0, 0]
                                    for byt_v in range(0,4):
                                        #dev_data[byt_v] = out_binary[byte_verifier]
                                        if out_binary[byte_verifier] != ord(dev_data[byt_v]):
                                            print "Did not verify:: Source: " + format(out_binary[byte_verifier], "02x") + ", Target: " + format(ord(dev_data[byt_v]), "02x") + " @" + format(byte_checker-4, "08x")
                                            #print "DID NOT VERIFY"
                                            is_fatal_error = True
                                            break
                                        byte_verifier += 1
                        if not is_fatal_error:
                            print "Write complete! -> '" + var_address + "'"
                        else:
                            print "ERROR in firmware verification!!!"
                    except SerialException:
                        print "ERROR: Port was not opened"
                except:
                    print "ERROR: Address Parse Error"
            else:
                print "No COM port or Binary file loaded"
        elif arg_set[IDX_COMM].lower() == 'connect' and len(arg_set) == 2:
            if ser != None:
                ser.close()
            try:
                ser = serial.Serial(arg_set[1], 115200, timeout=1)
                ser.write(b'V#')
                dev_version = str(ser.read(64))
                print ser.name + " -> " + dev_version
            except SerialException:
                print "ERROR: Port was not opened"
        elif arg_set[IDX_COMM].lower() == 'close' and len(arg_set) == 1:
            if ser != None:
                ser.close()
                print "Port closed..."
            else:
                print "No active port was found"
        else:
            print "Command (" + arg_set[IDX_COMM] + ") not found"
    else:
        print "Input Error"
    in_arg = raw_input('>> ')
    in_arg = str(in_arg)
if ser != None:
    ser.close()
