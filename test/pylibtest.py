import embeddedMylib

def parameter(objectToSet:embeddedMylib.SimpleIO, value:int):
    toret = objectToSet.I
    objectToSet.I = value
    toret2 = objectToSet.I
    return toret, toret2

def getterSetter(objectToSet:embeddedMylib.SimpleIO, value:int):
    toret = objectToSet.getI()
    objectToSet.setI(value)
    toret2 = objectToSet.getI()
    return toret, toret2

def setterdict(dictToset:dict,name, value:int):
    dictToset[name]=value