""" Provides Holder class defintion for general use.

Module content
--------------
"""
# The python-cctalk package allows one to send ccTalk messages and decode replies from a coin validator. 
license_text = "(C) 2011 David Schryer GNU GPLv3 or later."
__copyright__ = license_text

__autodoc__ = ['Holder']
__all__ = __autodoc__

class Holder(object):
    """This is a loose implimentation of  a dictionary object.

    It gives access to its items as member variables of a class.
    """
    def __init__(self, item=None):

        if item:
            self.add(item)

    def add(self, item):
        """Adds an item to the Holder.

        Parameters
        ----------
        item : dict or Holder
          Adds a dictionary or another Holder to this object.

        Raises
        ------
        UserWarning
          This is raised if item is another type.
        """
        if isinstance(item, dict):
            self.__dict__.update(item)
            
        elif isinstance(item, Holder):
            self.__dict__.update(item.__dict__)
            
        else:
            msg = "This Holder only supports dictionaries and other Holder objects."
            raise UserWarning(msg, (item, type(item)))

    def __getitem__(self, key):
        return self.__dict__[key]

    def __setitem__(self, key, value):
        self.__dict__[key] = value

    def get(self, key, alternate=None):
        """Gets an item from the Holder.

        Parameters
        ----------
        key : object
          The key used to retrieve the item from the Holder.
        alternate : object, optional
          The type of object to be returned if the key is not found.
          Defaults to None.
          
        Returns
        -------
        item : object
          The object linked to key or alternate.
        """
        return self.__dict__.get(key, alternate)

    def has_key(self, key):
        """Tests to see if this key is in the Holder.

        Parameters
        ----------
        key : object
          The key to test if it is contained in the Holder.

        Returns
        -------
        has_key : bool
          The answer to the question above.
        """
        return self.__dict__.has_key(key)

    def keys(self):
        """Gives all of the keys in the Holder.

        Returns
        -------
        keys : list
          A list of the keys in the Holder.
        """
        return self.__dict__.keys()
    
    def items(self):
        """Gives the items in the Holder.

        Returns
        -------
        items : list of tuples
          A list of (key, value) pair tuples that give all keys and values in the Holder.
        """
        return self.__dict__.items()
    
    def values(self):
        """Gives the values in the Holder

        Returns
        -------
        values : list
          A list of the values in the Holder.
        """
        return self.__dict__.values()

