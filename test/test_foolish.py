import callEmbed


def test_callFoolishSUM():
    for a, b in [(2, 3), (3, 2), (100, 255), (1039, -558), (0, -1)]:
        assert callEmbed.foolishCallADD(a, b) == (a + b)
