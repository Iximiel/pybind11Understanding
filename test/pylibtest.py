import embeddedMylib


def parameter(objectToSet: embeddedMylib.SimpleIO, value: int):
    toret = objectToSet.I
    objectToSet.I = value
    toret2 = objectToSet.I
    return toret, toret2


def getterSetter(objectToSet: embeddedMylib.SimpleIO, value: int):
    toret = objectToSet.getI()
    objectToSet.setI(value)
    toret2 = objectToSet.getI()
    return toret, toret2


def setterdict(dictToset: dict, name: str, value: int):
    dictToset[name] = value


def setterTupleLike(thingToset: "list or tuple", value0: int, value1: int):
    thingToset[0] = value0
    thingToset[1] = value1
