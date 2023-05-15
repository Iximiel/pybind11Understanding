import embeddedMylib

def setter(objectToSet:embeddedMylib.SimpleIO, value):
    toret = objectToSet.getI()
    objectToSet.setI(value)
    return toret

def setterdict(dictToset:dict,name, value):
    dictToset[name]=value