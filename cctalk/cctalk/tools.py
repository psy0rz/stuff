""" Provides tools for general use.

Module content
--------------
"""
# The python-cctalk package allows one to send ccTalk messages and decode replies from a coin validator. 
license_text = "(C) 2011 David Schryer GNU GPLv3 or later."
__copyright__ = license_text

__autodoc__ = ['make_serial_object', 'drop_to_ipython', 'make_msg', 'send_packet_and_get_reply', 'interpret_reply']
__all__ = __autodoc__

from IPython.frontend.terminal.embed import InteractiveShellEmbed

import os
import serial
import time
import subprocess
from struct import unpack

def make_msg(code, data=None, to_slave_addr=2, from_host_addr=1):
    """Makes a ccTalk message from a ccTalk code and data to be sent with this packet.

    Parameters
    ----------
    code : int
      ccTalk code for this message.
    data : list of integers
      Data to be sent in this message.
    to_slave_addr : int
      Address of slave to be sent to.  Defaults to 2.
    from_host_addr : int
      Address of host that is sending the message.  Defaults to 1.

    Returns
    -------
    packet : list of integers
      An integer equivalent of the ccTalk packet.
      This needs to be converted to a byte packet prior to sending.
    """
    if not data:
        seq = [to_slave_addr, 0, from_host_addr, code]
    else:
        seq = [to_slave_addr, len(data), from_host_addr, code] + data
    packet_sum = 0
    for i in seq:
        packet_sum += i
    end_byte = 256 - (packet_sum%256)
    packet = seq + [end_byte]
    return packet

def send_packet_and_get_reply(serial_object, packet_holder, initial_wait=0.05, total_wait=1,
                              debug=True, verbose=True):
    """Sends a packet and gets a reply.

    Parameters
    ----------
    serial_object : object made with :py:func:`cctalk.tools.make_serial_object`
      Serial communication object.
    packet_holder : Holder
      Holder containing the packet and extended information about the packet being send.
      See :py:class:`cctalk.coin_messenger.CoinMessenger` for Holder construction.
    initial_wait : float
      Time in seconds before probing for a reply.  Defaults to 0.05 seconds.
    total_wait : float
      Time in seconds before giving up.  Defaults to 1 second.
    debug : bool
      Flag to send out debug messages.
    verbose : bool
      Flag to be more verbose.

    Returns
    -------
    reply_msg : message recieved from :py:func:`cctalk.tools.interpret_reply`
      if reply_msg is False, no reply was obtained.

    Raises
    ------
    UserWarning
      If a reply was obtained but :py:func:`cctalk.tools.interpret_reply` returned False.
    """
    h = packet_holder
    packet = h.packet
    byte_msg = h.byte_message

    s = time.time()
    serial_object.write(packet)
    time.sleep(initial_wait)
    while True:
        t = time.time() - s
        if t > total_wait: break

        raw = serial_object.read(serial_object.inWaiting())
        if len(raw) > 1:
            len_raw = len(raw)
            out_byte = unpack('={0}c'.format(int(len_raw)), raw)
            out_int = map(ord, out_byte)

            
            if len(out_byte) == len(byte_msg) and debug:
                pass #print 'Recieved original packet int: {0}   byte:{1}'.format(out_int, byte_msg)
            elif len(out_byte) < len(byte_msg) and debug:
                pass #print 'Recieved small packet int: {0}   byte:{1}'.format(out_int, byte_msg)
            else:
                # The first part of the return is the echo in the line
                # (a repeat of the message sent).
                start_index = 5 + h.bytes_sent
                reply_packet = out_byte[start_index:]

                reply_msg = interpret_reply(reply_packet, packet_holder, verbose=verbose)

                if reply_msg:
                    return reply_msg
                else:
                    msg = "A bad reply was recieved."
                    raise UserWarning(msg, (reply_packet, reply_msg))
    
    return False

def interpret_reply(reply_byte, packet_holder, verbose=False):
    """Interprets a reply byte message.

    Parameters
    ----------
    reply_byte : byte message returned from serial_object.read(serial_object.inWaiting())
      Often the reply contains an echo of the message sent.  This part should be removed first.
    packet_holder : Holder
      Holder containing the packet and extended information about the packet that was originally sent.
      See :py:class:`cctalk.coin_messenger.CoinMessenger` for Holder construction.
    verbose : bool
      Flag to be more verbose.

    Returns
    -------
    reply : The type dependes on the type expected.
      Reply to the message in the type expected.

    Raises
    ------
    UserWarning
      If a simple pool did not return an expected message.
      Assumes 1,2 for send recieve hosts.
    """
    h = packet_holder
    reply_length = h.bytes_returned
    reply_type = h.type_returned
    reply_int = map(ord, reply_byte)

    if len(reply_int) < 2:
        print 'Recieved small packet int: {0}   byte:{1}'.format(reply_int, reply_byte)
        return False

    msg_length = reply_int[1]

    if msg_length != reply_length:
        #print 'Message length != return_length.  ml: {0}   rl:{1}'.format(msg_length, reply_length)
        return False


    if h.request_code == 254:
        expected_reply = [1, 0, 2, 0, 253]
        if reply_int != expected_reply:
            msg = "Simple pool did not return expected message."
            raise UserWarning(msg, (reply_int, expected_reply))

    reply_msg_int = reply_int[4:-1]
    reply_msg_byte = reply_byte[4:-1]
    
    if reply_type is str:
        return str().join(reply_msg_byte)
    elif reply_type is int:
        return reply_msg_int
    elif reply_type is bool:
        return True
    else:
        return reply_msg_byte

def _get_tty_port(port_type):
    """Returns the tty device name for 'coin_validator'.

    Parameters
    ----------
    port_type : str
      Type of port to connect to.  Currently only 'coin_validator is valid.'

    Notes
    -----
    Only used within this module.

    Raises
    ------
    NotImplementedError
      If a valid port_type is not specified.
    """
    return ("ttyUSB0")

    if port_type == 'coin_validator':
        usb_conn = '3-2'
    else:
        msg = "This port_type has not yet been implimented yet."
        raise NotImplementedError(msg, (port_type))

    cmd = 'lshal |grep sysfs | grep ttyUSB | grep {0}'.format(usb_conn)

    proc = subprocess.Popen(cmd,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            env=os.environ)
    out, err = proc.communicate()

    out_string = out.split('/tty/')
    if len(out_string) != 2:
        msg = "No USB device {0} was found ({1}).  Look at: lshal | grep sysfs | grep ttyUSB".format(usb_conn, port_type)
        raise UserWarning(msg, (out))

    return out_string[1].split("'")[0]


def make_serial_object(port):
    """Makes a serial object that can be used for talking with the coin validator.

    port_type is a string that can currently only be equal to 'coin_validator'.

    Paramters
    ---------
    port_type : str
      Type of port to connect to.  Currently only 'coin_validator is valid.'

    Returns
    -------
    serial_object : object made by :py:class:`serial.Serial`
    """
    
    return serial.Serial(port=port,
                         baudrate=9600,
                         parity=serial.PARITY_NONE,
                         stopbits=serial.STOPBITS_ONE,
                         bytesize=serial.EIGHTBITS,
                         xonxoff=True,
                         )

def drop_to_ipython(local_variables, *variables_to_inspect):
    '''
    Drops to ipython at the point in the code where it is called to inspect the variables passed to it.

    Parameters
    ----------
    local_variables : list
      Usually one would pass the output of locals().
    variables_to_inspect: tuple
      All variables passed to this routine are wrapped into a tuple.
    '''

    try:
        call_name = local_variables['self'].__module__
    except Exception:
        call_name = "Module"

    b = 'Dropping into IPython'
    em = 'Leaving Interpreter, back to program.'
    msg = '***Called from %s. Hit Ctrl-D to exit interpreter and continue program.'
    ipshell = InteractiveShellEmbed([], banner1=b, exit_msg=em)
    ipshell(msg %(call_name))

