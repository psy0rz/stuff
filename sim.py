#!/usr/bin/env python3

'''economy simulator 1.0 - (C)2013 EH.Eefting'''

import time


class Economy(object):
    '''a complete econonomy. contains all the units and runs the simulation step by step'''
    def __init__(self):

        return





class Item(object):
    '''one or more items, like money, kilometers, breads, food items, lifetime-hours, labour hours'''

    def __init__(self, name, ttl=None, amount=1, created=time.time()):
        self.name=name
        self.amount=amount;
        self.created=created
        self.ttl=ttl

    def __str__(self):
        return ("Item {}({})".format(self.name, self.amount))

    def split(self, amount):
        '''split the item up by returning a copy of the item with the specified amount, and substracting that amount from this item.'''

        if (amount>=self.amount):
            raise(Exception("Not enough items left"))

        self.amount=self.amount-amount        

        new_item=Item(self.name, self.ttl, amount, self.created);

        return (new_item)


class Transaction():
    '''a transaction of items from a source to a destination unit'''


class SellableItem():
    '''item that can be 'selled' to another unit by exchanging other items (like money or goods)

    this is done by completing all the transactions
    '''

    #transfer this amount of items per 1 Item


    def __init__(self):
        super(SellableItem, self).__init__(name)



class ItemCollection(object):
    '''collection of items, indexed by name and sorted by ttl left'''

    def __init__(self):
        self.items={}


    def __str__(self):
        ret=""
        for item_list in self.items.values():
            for item in item_list:
                ret=ret+str(item)
            ret=ret+"\n"
        return ret

    def add(self, item):
        '''add item to collection'''
        if item.name not in self.items:
            self.items[item.name]=[]

            self.items[item.name].append(item)

    def amount(self, name):
        '''check amount we have of a certain item'''

        if name not in self.items:
            return 0

        amount=0
        for item in self.items[name]:
            amount=amount+item.amount            

    def substract(self, name, amount=None):
        '''substract a certain amount of items from the collection. returns a list of item objects, that will have the requested amount when combined (if there are enough)

        use amount=None to pop all items with this name
        '''

        if name not in self.items:
            return []


        #return all items
        if (amount==None):
            ret=self.items[name]
            self.items[name]=[]
            return(ret)

        item_list=self.items[name]

        ret=[]
        amount_left=amount
        while amount_left>0 and len(item_list)>0:
            if item_list[0].amount>amount_left:
                ret.append(item_list[0].split(amount_left))
                amount_left=0
            else:
                amount_left=amount_left-item_list[0].amount
                ret.append(item_list.pop())

        return ret

 



class Process(object):
    '''a process: converts items from one form to another, within a unit. 
    '''

    def __init(self, name):
        self.name=name

class ProcessGenerator(Process):
    '''generator process: a process that keeps converting items at a certain rate'''

    def __init(self, name):

        super(ProcessGenerator, self).__init__(name)


class Unit(object):
    '''a unit, can be a person or a business. has ownership over items and processes'''

    def __init__(self, name, ttl=None):
        self.name=name
        self.created=time.time()
        self.ttl=ttl
        self.items=ItemCollection()




e=Item(name='euro', amount=10)
h=Item(name='hour', amount=5)

#print (e,h)

c=ItemCollection()

c.add(e)
c.add(h)

print (c)

print ("af:")
print (c.substract('euro',2)[0])

print ("over:")
print (c)

 
