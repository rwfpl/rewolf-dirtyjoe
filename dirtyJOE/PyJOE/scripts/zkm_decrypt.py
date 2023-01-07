################################################################################
#
# Sample string decrypter for dirtyJOE, with proper key it can decrypt strings
# from files obfuscated by Zelix KlassMaster.
#
# Copyright 2011 ReWolf
#
################################################################################

import pyjoe

def dj_decryptUTF8(inBuf):
	key = [1, 2, 3, 4, 5]
	return pyjoe.dj_decryptUTF8_ZKM(inBuf, key)
