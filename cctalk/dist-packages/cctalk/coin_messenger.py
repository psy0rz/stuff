""" Provides the CoinMessenger class definition.

Module content
--------------
"""
# The python-cctalk package allows one to send ccTalk messages and decode replies from a coin validator. 
license_text = "(C) 2011 David Schryer GNU GPLv3 or later."
__copyright__ = license_text

__autodoc__ = ['CoinMessenger']
__all__ = __autodoc__

import time

from struct import pack, calcsize
from . tools import make_serial_object, make_msg, send_packet_and_get_reply, interpret_reply
from . holder import Holder

class CoinMessenger(object):
    """This is an object used to talk with ccTalk coin validators.

    It provides functions for requesting and recieving data as
    well as changing the state of the coin validator.
    """
    
    r_info = dict(reset_device=(1, 0, bool),
                  comms_revision=(4, 3, int),              # Expected: 2, 4, 2
                  request_build_code=(192, 3, str),
                  data_storage_availability=(216, 5, int),
                  read_buffered_credit_or_error_codes=(229, 11, int),
                  master_inhibit_status=(227, 1, int),
                  inhibit_status=(230, 2, int),
                  software_revision=(241, 5, str),         # Expected: 52.05
                  serial_number=(242, 3, int),
                  product_type=(244, 3, str),              # Expected: G13
                  equiptment_category=(245, 13, str),      # Expected: Coin Acceptor
                  manufacturer_ID=(246, 3, str),           # Expected: NRI   Gets NR with I as checksum.
                  simple_poll=(254, 0, bool),
                  )

    
    def __init__(self, serial_object):
        self.serial_object = serial_object
        self.request_data = {}
        for k, v in self.r_info.items():
            int_msg = make_msg(v[0])
            byte_msg = map(chr, int_msg)
            packet = pack('=ccccc', *byte_msg)

            self.request_data[k] = Holder(dict(packet=packet,
                                               integer_message=int_msg,
                                               byte_message=byte_msg,
                                               request_code=v[0],
                                               bytes_returned=v[1],
                                               bytes_sent=0,
                                               type_returned=v[2],
                                               user_message=k,
                                               ))

    def accept_coins(self, state=None, verbose=True):
        """Change accept coin state.

        Parameters
        ----------
        state : bool
          State to change the coin validator too.
        verbose : bool
          Flag to specify verbosity.

        Raises
        ------
        UserWarning
          -- If the state is not True or False.
          -- If self.serial_object.isOpen() is False.
        """
        if state is None:
            state = True

        if not state and state != False:
            msg = 'The state must be either True or False.'
            raise UserWarning(msg, (state))

        if state:
            s_msg = 'on'
        else:
            s_msg = 'off'
            
        int_msg = make_msg(231, [255, 255])
        byte_msg = map(chr, int_msg)
        packet = pack('=ccccccc', *byte_msg)

        if not self.serial_object.isOpen():
            msg = "The serial_object.isOpen() is False."
            raise UserWarning(msg, (self.serial_object.isOpen()))

        ph = Holder(dict(packet=packet,
                         integer_message=int_msg,
                         byte_message=byte_msg,
                         request_code=231,
                         bytes_returned=0,
                         bytes_sent=2,
                         type_returned=bool,
                         user_message='modify_inhibit_status_{0}'.format(s_msg),
                         ))
        #print 'Requesting: {0}'.format(ph.user_message)
        reply_msg = send_packet_and_get_reply(self.serial_object, ph, verbose=verbose)
        #print reply_msg

    def set_accept_limit(self, coins=1, verbose=True):
        """Sets the accept limit of the coin validator.

        Parameters
        ----------
        coins : int
          Number of coins.  Defaults to 1.
        verbose : bool
          Flag to be more verbose.
        """
        if type(coins) != type(int()):
            msg = 'The number of coins must be an integer.'
            raise UserWarning(msg, (coins, type(coins)))

        int_msg = make_msg(135, [coins])
        byte_msg = map(chr, int_msg)
        packet = pack('=cccccc', *byte_msg)

        if not self.serial_object.isOpen():
            msg = "The serial_object.isOpen() is False."
            raise UserWarning(msg, (self.serial_object.isOpen()))

        ph = Holder(dict(packet=packet,
                         integer_message=int_msg,
                         byte_message=byte_msg,
                         request_code=135,
                         bytes_returned=0,
                         bytes_sent=1,
                         type_returned=bool,
                         user_message='set_accept_limit_{0}'.format(coins),
                         ))
        #print 'Requesting: {0}'.format(ph.user_message)
        reply_msg = send_packet_and_get_reply(self.serial_object, ph, verbose=verbose)
        #print reply_msg

    def read_buffer(self):
        """Shortcut for self.request('read_buffered_credit_or_error_codes')

        Returns
        -------
        output : output from self.request('read_buffered_credit_or_error_codes')
        """
        return self.request('read_buffered_credit_or_error_codes')

    def get_coin_id(self, slot, verbose=True):
        """Prints out the coin id for a slot number.

        Parameters
        ----------
        slot : int
          Slot number.
        verbose : bool
          Flag to be more verbose.
        """
        int_msg = make_msg(184, [slot])
        byte_msg = map(chr, int_msg)
        packet = pack('=cccccc', *byte_msg)

        if not self.serial_object.isOpen():
            msg = "The serial_object.isOpen() is False."
            raise UserWarning(msg, (self.serial_object.isOpen()))

        ph = Holder(dict(packet=packet,
                         integer_message=int_msg,
                         byte_message=byte_msg,
                         request_code=184,
                         bytes_returned=6,
                         bytes_sent=1,
                         type_returned=str,
                         user_message='get_coin_id_{0}'.format(slot),
                         ))
        #print 'Requesting: {0}'.format(ph.user_message)
        reply_msg = send_packet_and_get_reply(self.serial_object, ph, verbose=verbose)
        return(reply_msg)

    def request(self, request_key, verbose=True):
        """Requests messages programmed into the request Holder in __init__.

        Parameters
        ----------
        request_key : key
          Key in the request Holder.
        verbose : bool
          Flag to be more verbose.

        Returns
        -------
        reply_msg : reply message from :py:func:`cctalk.tools.send_packet_and_get_reply`

        Raises
        ------
        NotImplimentedError
          If the key is not in the request Holder.
        UserWarning
          If self.serial_object.isOpen() is False
        """
        r_dic = self.request_data.get(request_key, None)
        if not r_dic:
            msg = 'This request_key has not been implimented.'
            raise NotImplimentedError(msg, (request_key))

        if not self.serial_object.isOpen():
            msg = "The serial_object.isOpen() is False."
            raise UserWarning(msg, (self.serial_object.isOpen()))
        
        #print 'Requesting: {0}'.format(r_dic.user_message)
        reply_msg = send_packet_and_get_reply(self.serial_object, r_dic, verbose=verbose)

        return reply_msg
        
