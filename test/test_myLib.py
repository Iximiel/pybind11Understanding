import mypylib


def test_GetSet():
    temp = mypylib.SimpleIO()
    assert temp.getI() == 0
    for setNum in [1, 56, 33, 99, 869]:
        temp.setI(setNum)
        assert temp.getI() == setNum
